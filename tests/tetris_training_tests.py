"""CEM update, split isolation, profile loading and repeatable end-to-end training."""
import csv
import importlib.util
import io
import json
import math
from pathlib import Path
import subprocess
import sys
import tempfile

engine, trainer_path = Path(sys.argv[1]).resolve(), Path(sys.argv[2]).resolve()
spec = importlib.util.spec_from_file_location('tetris_training', trainer_path)
trainer = importlib.util.module_from_spec(spec)
spec.loader.exec_module(trainer)

mean, deviation = [0.0, 0.0], [2.0, 2.0]
trainer.update_distribution(mean, deviation, [[2.0, 3.0], [4.0, 3.0]], 0.5, 0.2)
assert mean == [1.5, 1.5]
assert math.isclose(deviation[0], math.sqrt(2.5)) and math.isclose(deviation[1], math.sqrt(2.0))
trainer.update_distribution(mean, deviation, [[1.0, 1.0]] * 2, 1.0, 0.2)
assert mean == [1.0, 1.0] and deviation == [0.2, 0.2]
weights = trainer.parameters_to_weights([1e9, -1e9, 0.0, 1.0])
assert weights[0] == 100 and all(-1000 <= value < 0 for value in weights[1:])


def game(*args):
    return subprocess.run([str(engine), *map(str, args)], capture_output=True, text=True, timeout=30)


with tempfile.TemporaryDirectory() as temporary:
    root = Path(temporary)
    profile = root / 'profile with spaces.weights'
    trainer.write_weights(profile, trainer.BASELINE)
    valid_text = profile.read_text()
    baseline = game('--seed', 1, '--games', 2)
    loaded = game('--seed', 1, '--games', 2, '--weights', profile)
    assert baseline.returncode == loaded.returncode == 0
    assert baseline.stdout == loaded.stdout
    rows = list(csv.DictReader(io.StringIO(loaded.stdout)))
    assert [(r['lines'], r['score']) for r in rows] == [('13', '3280'), ('52', '27127')]
    bad_profiles = [valid_text.replace('V1', 'V9'), valid_text.replace('holes -80', 'holes nan'),
                    valid_text.replace('holes -80', 'holes -inf'), valid_text.replace('height -5', 'height -1001'),
                    valid_text.replace('lines 100', 'lines 0'), valid_text.replace('holes -80', 'holes 1'),
                    valid_text.replace('holes -80', 'holes -1e999'), valid_text + 'extra 1\n',
                    valid_text.replace('holes -80\n', ''), valid_text.replace('holes', 'height'),
                    valid_text.replace('holes -80', 'holes ' + '9' * 200), '']
    for text in bad_profiles:
        profile.write_text(text)
        result = game('--weights', profile)
        assert result.returncode == 1 and result.stderr and not result.stdout, result
    assert game('--weights').returncode == 1
    assert game('--weights', root / 'missing.weights').returncode == 1
    trainer.write_weights(profile, (100, 0, 0, 0, 0))
    altered = game('--seed', 1, '--games', 2, '--weights', profile)
    assert altered.returncode == 0 and altered.stdout != baseline.stdout
    outputs = [root / 'one-worker', root / 'two-workers']
    for workers, output in enumerate(outputs, 1):
        command = [sys.executable, str(trainer_path), '--engine', str(engine), '--output', str(output),
                   '--iterations', '2', '--population', '4', '--train-games', '2',
                   '--validation-games', '3', '--test-games', '4', '--pieces', '80', '--workers', str(workers)]
        result = subprocess.run(command, capture_output=True, text=True, timeout=90)
        assert result.returncode == 0, result
        report = json.loads((output / 'report.json').read_text())
        history = json.loads((output / 'history.json').read_text())
        assert len(history) == 2 and report['selected_generation'] in (0, 1, 2)
        assert report['validation']['learned']['mean_lines'] >= report['validation']['baseline']['mean_lines']
        train_seeds, validation_seeds, test_seeds = set(), set(), set()
        for path in (output / 'checkpoints').glob('*-training.csv'):
            records = list(csv.DictReader(io.StringIO(path.read_text())))
            by_policy = {}
            for row in records:
                by_policy.setdefault(row['policy'], []).append(int(row['seed']))
            assert len(by_policy) == 4 and all(len(seeds) == 2 for seeds in by_policy.values())
            assert all(seeds == next(iter(by_policy.values())) for seeds in by_policy.values())
            train_seeds.update(next(iter(by_policy.values())))
        for row in csv.DictReader(io.StringIO((output / 'validation.csv').read_text())):
            validation_seeds.add(int(row['seed']))
        test_rows = list(csv.DictReader(io.StringIO((output / 'test.csv').read_text())))
        for row in test_rows:
            test_seeds.add(int(row['seed']))
        assert len(train_seeds) == 4 and len(validation_seeds) == 3 and len(test_seeds) == 4
        assert not train_seeds & validation_seeds and not train_seeds & test_seeds and not validation_seeds & test_seeds
        replay = game('--weights', output / 'best.weights', '--seed', trainer.TEST_START, '--games', 4, '--pieces', 80)
        assert replay.returncode == 0
        expected = [{k: v for k, v in row.items() if k != 'policy'} for row in test_rows if row['policy'] == 'learned']
        assert list(csv.DictReader(io.StringIO(replay.stdout))) == expected
        before = (output / 'best.weights').read_bytes()
        duplicate = subprocess.run(command, capture_output=True, text=True, timeout=10)
        assert duplicate.returncode == 1 and (output / 'best.weights').read_bytes() == before
    for artifact in ('best.weights', 'test.csv', 'validation.csv', 'history.json'):
        assert (outputs[0] / artifact).read_bytes() == (outputs[1] / artifact).read_bytes(), artifact
print('PASS weight parsing, original seed scores, CEM updates, seed isolation, saved profiles and worker-independent training')
