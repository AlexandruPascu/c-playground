#!/usr/bin/env python3
"""Cross-entropy policy search using the C Tetris simulator (Python stdlib only)."""
import argparse
from concurrent.futures import ThreadPoolExecutor
import csv
import hashlib
import io
import json
import math
from pathlib import Path
import platform
import random
import statistics
import subprocess
import sys

FEATURES = ('lines', 'height', 'holes', 'bumpiness', 'max_height')
BASELINE = (100.0, -5.0, -80.0, -3.0, -2.0)
TRAIN_START, VALIDATION_START, TEST_START = 1001, 1000001, 2000001
LOG_MIN, LOG_MAX = math.log(0.001), math.log(1000.0)


def write_json(path, value):
    temporary = path.with_suffix(path.suffix + '.tmp')
    temporary.write_text(json.dumps(value, indent=2, allow_nan=False) + '\n', encoding='utf-8')
    temporary.replace(path)


def write_weights(path, weights):
    text = 'TETRIS_WEIGHTS_V1\n' + ''.join(f'{name} {value:.17g}\n' for name, value in zip(FEATURES, weights))
    temporary = path.with_suffix(path.suffix + '.tmp')
    temporary.write_text(text, encoding='ascii')
    temporary.replace(path)


def parameters_to_weights(parameters):
    # A positive scale factor cannot change argmax. Fix lines=100 and search four
    # relative penalty magnitudes in log space, retaining the known feature signs.
    return (100.0, *(-math.exp(min(LOG_MAX, max(LOG_MIN, p))) for p in parameters))


def percentile(values, fraction):
    ordered = sorted(values)
    position = (len(ordered) - 1) * fraction
    low, high = math.floor(position), math.ceil(position)
    return ordered[low] + (ordered[high] - ordered[low]) * (position - low)


def summarize(rows):
    lines = [row['lines'] for row in rows]
    return dict(games=len(rows), mean_lines=statistics.mean(lines), median_lines=statistics.median(lines),
                p10_lines=percentile(lines, 0.1), min_lines=min(lines), max_lines=max(lines),
                mean_pieces=statistics.mean(row['pieces'] for row in rows),
                mean_score=statistics.mean(row['score'] for row in rows),
                reached_piece_cap=sum(row['status'] == 'limit' for row in rows))


def rank(rows):
    stats = summarize(rows)
    return stats['mean_lines'], stats['median_lines'], stats['p10_lines']


def update_distribution(mean, deviation, elite, smoothing, floor):
    for dimension in range(len(mean)):
        values = [point[dimension] for point in elite]
        center = statistics.mean(values)
        variance = statistics.pvariance(values)
        mean[dimension] = (1 - smoothing) * mean[dimension] + smoothing * center
        deviation[dimension] = max(floor, math.sqrt((1 - smoothing) * deviation[dimension] ** 2 + smoothing * variance))


def evaluate(engine, weights_file, start, games, pieces):
    result = subprocess.run([str(engine), '--weights', str(weights_file), '--seed', str(start),
                             '--games', str(games), '--pieces', str(pieces)],
                            capture_output=True, text=True, timeout=180)
    if result.returncode:
        raise RuntimeError(f'Simulator failed ({result.returncode}): {result.stderr.strip()}')
    reader = csv.DictReader(io.StringIO(result.stdout))
    if reader.fieldnames != ['seed', 'pieces', 'lines', 'score', 'level', 'status']:
        raise RuntimeError('Unexpected simulator CSV header')
    rows = []
    for row in reader:
        parsed = {name: int(row[name]) for name in reader.fieldnames[:-1]}
        parsed['status'] = row['status']
        if (parsed['seed'] != start + len(rows) or not 0 <= parsed['lines'] <= parsed['pieces'] * 4 // 10
                or not 1 <= parsed['pieces'] <= pieces or parsed['score'] < 0
                or parsed['level'] != min(20, 1 + parsed['lines'] // 10)
                or parsed['status'] not in ('limit', 'game_over')
                or (parsed['status'] == 'limit' and parsed['pieces'] != pieces)):
            raise RuntimeError(f'Invalid simulator result: {parsed}')
        rows.append(parsed)
    if len(rows) != games:
        raise RuntimeError('Simulator returned the wrong number of games')
    return rows


def save_rows(path, policies):
    with path.open('w', newline='', encoding='utf-8') as stream:
        writer = csv.DictWriter(stream, fieldnames=['policy', 'seed', 'pieces', 'lines', 'score', 'level', 'status'])
        writer.writeheader()
        for policy, rows in policies.items():
            for row in rows:
                writer.writerow(dict(policy=policy, **row))


def paired_comparison(baseline, learned):
    assert [row['seed'] for row in baseline] == [row['seed'] for row in learned]
    differences = [new['lines'] - old['lines'] for old, new in zip(baseline, learned)]
    rng = random.Random(7829)  # Independent of the optimizer's RNG.
    bootstrap = [statistics.mean(rng.choices(differences, k=len(differences))) for _ in range(2000)]
    return dict(mean_line_difference=statistics.mean(differences),
                mean_line_difference_ci95=[percentile(bootstrap, 0.025), percentile(bootstrap, 0.975)],
                wins=sum(d > 0 for d in differences), ties=sum(d == 0 for d in differences),
                losses=sum(d < 0 for d in differences))


def bounded_int(low, high):
    def parse(text):
        try:
            value = int(text)
        except ValueError as error:
            raise argparse.ArgumentTypeError('Expected an integer') from error
        if not low <= value <= high:
            raise argparse.ArgumentTypeError(f'Expected {low}..{high}')
        return value
    return parse


def parse_args(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--engine', type=Path, required=True, help='Release-build tetris_ai executable')
    parser.add_argument('--output', type=Path, required=True, help='New directory for profiles, checkpoints and reports')
    parser.add_argument('--iterations', type=bounded_int(1, 1000), default=12)
    parser.add_argument('--population', type=bounded_int(4, 256), default=32)
    parser.add_argument('--train-games', type=bounded_int(1, 100), default=12)
    parser.add_argument('--validation-games', type=bounded_int(1, 100), default=32)
    parser.add_argument('--test-games', type=bounded_int(1, 100), default=100)
    parser.add_argument('--pieces', type=bounded_int(1, 100000), default=1000)
    parser.add_argument('--seed', type=bounded_int(0, 2147483647), default=23, help='Optimizer RNG seed; game splits are separate')
    parser.add_argument('--workers', type=bounded_int(1, 32), default=4)
    return parser.parse_args(argv)


def train(args):
    engine = args.engine.resolve(strict=True)
    if not engine.is_file():
        raise ValueError('Engine must be an executable file')
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=False)
    candidates = output / 'candidates'
    checkpoints = output / 'checkpoints'
    candidates.mkdir(); checkpoints.mkdir()
    config = {key: str(value) if isinstance(value, Path) else value for key, value in vars(args).items()}
    config.update(engine=str(engine), output=str(output), elite_fraction=0.25, smoothing=0.7,
                  initial_log_std=1.0, min_log_std=0.2, objective='mean cleared lines at a fixed piece cap',
                  train_seed_start=TRAIN_START, validation_seed_start=VALIDATION_START, test_seed_start=TEST_START)
    # The fixed ranges cannot overlap under the command-line bounds.
    assert TRAIN_START + args.iterations * args.train_games <= VALIDATION_START
    assert VALIDATION_START + args.validation_games <= TEST_START
    provenance = dict(engine_sha256=hashlib.sha256(engine.read_bytes()).hexdigest(),
                      trainer_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
                      python=platform.python_version(), platform=platform.platform())
    source_root = Path(__file__).resolve().parents[1]
    provenance['engine_sources_sha256'] = {
        path.name: hashlib.sha256(path.read_bytes()).hexdigest()
        for path in sorted((source_root / 'Lab05/Solved').glob('*.[ch]'))}
    write_json(output / 'config.json', dict(config=config, provenance=provenance))
    rng = random.Random(args.seed)
    mean = [math.log(-weight) for weight in BASELINE[1:]]
    deviation = [config['initial_log_std']] * 4
    champion_parameters = mean.copy()
    champion_weights = BASELINE
    baseline_file = output / 'baseline.weights'
    best_file = output / 'best.weights'
    write_weights(baseline_file, BASELINE)
    write_weights(best_file, champion_weights)
    baseline_validation = evaluate(engine, baseline_file, VALIDATION_START, args.validation_games, args.pieces)
    champion_validation = baseline_validation
    selected_generation = 0
    history = []
    print(f'Baseline validation: mean {rank(baseline_validation)[0]:.2f} lines', flush=True)
    with ThreadPoolExecutor(max_workers=args.workers) as pool:
        for generation in range(1, args.iterations + 1):
            start = TRAIN_START + (generation - 1) * args.train_games
            parameters = [mean.copy()]
            if champion_parameters != mean:
                parameters.append(champion_parameters.copy())
            while len(parameters) < args.population:
                parameters.append([min(LOG_MAX, max(LOG_MIN, rng.gauss(mu, sigma))) for mu, sigma in zip(mean, deviation)])
            files = []
            for index, point in enumerate(parameters):
                path = candidates / f'g{generation:03d}-p{index:03d}.weights'
                write_weights(path, parameters_to_weights(point))
                files.append(path)
            futures = [pool.submit(evaluate, engine, path, start, args.train_games, args.pieces) for path in files]
            results = [future.result() for future in futures]  # Stable order, independent of worker completion order.
            order = sorted(range(args.population), key=lambda index: rank(results[index]), reverse=True)
            winner = order[0]
            elite_count = max(2, math.ceil(args.population * config['elite_fraction']))
            update_distribution(mean, deviation, [parameters[i] for i in order[:elite_count]],
                                config['smoothing'], config['min_log_std'])
            validation = evaluate(engine, files[winner], VALIDATION_START, args.validation_games, args.pieces)
            if rank(validation) > rank(champion_validation):
                champion_parameters = parameters[winner].copy()
                champion_weights = parameters_to_weights(champion_parameters)
                champion_validation = validation
                selected_generation = generation
                write_weights(best_file, champion_weights)
            entry = dict(generation=generation, train_seed_start=start, train_seed_end=start + args.train_games - 1,
                         train_winner=summarize(results[winner]), validation_winner=summarize(validation),
                         validation_best=summarize(champion_validation), selected_generation=selected_generation,
                         winner_weights=parameters_to_weights(parameters[winner]))
            history.append(entry)
            write_weights(checkpoints / f'generation-{generation:03d}.weights', champion_weights)
            write_json(checkpoints / f'generation-{generation:03d}.json',
                       dict(config=config, generation=generation, mean=mean, log_std=deviation,
                            rng_state=rng.getstate(), champion_weights=champion_weights,
                            selected_generation=selected_generation, metrics=entry))
            save_rows(checkpoints / f'generation-{generation:03d}-training.csv',
                      {f'candidate-{i:03d}': rows for i, rows in enumerate(results)})
            save_rows(checkpoints / f'generation-{generation:03d}-validation.csv', {'winner': validation})
            write_json(output / 'history.json', history)
            print(f'Generation {generation:02d}/{args.iterations}: train {rank(results[winner])[0]:.2f}, '
                  f'validation {rank(validation)[0]:.2f}, best {rank(champion_validation)[0]:.2f} lines', flush=True)
    # Test seeds are evaluated only after model selection is finished. They never update the model.
    baseline_test = evaluate(engine, baseline_file, TEST_START, args.test_games, args.pieces)
    learned_test = evaluate(engine, best_file, TEST_START, args.test_games, args.pieces)
    save_rows(output / 'test.csv', {'baseline': baseline_test, 'learned': learned_test})
    save_rows(output / 'validation.csv', {'baseline': baseline_validation, 'learned': champion_validation})
    report = dict(schema_version=1, method='cross-entropy search over log penalty magnitudes',
                  config=config, provenance=provenance, selected_generation=selected_generation,
                  weights=dict(zip(FEATURES, champion_weights)),
                  validation=dict(baseline=summarize(baseline_validation), learned=summarize(champion_validation)),
                  test=dict(seed_start=TEST_START, seed_end=TEST_START + args.test_games - 1,
                            baseline=summarize(baseline_test), learned=summarize(learned_test),
                            paired=paired_comparison(baseline_test, learned_test)))
    write_json(output / 'report.json', report)
    baseline_mean, learned_mean = rank(baseline_test)[0], rank(learned_test)[0]
    print(f'Held-out test: baseline {baseline_mean:.2f}, learned {learned_mean:.2f} mean lines. '
          f'Profile: {best_file}', flush=True)
    return report


def main(argv=None):
    args = parse_args(argv)
    try:
        train(args)
    except (OSError, ValueError, RuntimeError, subprocess.TimeoutExpired) as error:
        print(f'Training failed: {error}', file=sys.stderr)
        return 1
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
