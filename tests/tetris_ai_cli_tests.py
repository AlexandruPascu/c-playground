"""Replayable headless games and bounded command-line input."""
import csv
import io
import subprocess
import sys

executable = sys.argv[1]
def run(*args):
    return subprocess.run([executable, *args], capture_output=True, text=True, timeout=30)

assert 'Usage:' in run('--help').stdout
for args in [('--seed',), ('--pieces', '0'), ('--pieces', '100001'), ('--seed', '-1'),
             ('--seed', '2147483648'), ('--games', '101'), ('--games', 'bad'), ('unexpected',),
             ('--seed', '2147483647', '--games', '2')]:
    result = run(*args)
    assert result.returncode == 1 and result.stderr, (args, result)
first = run('--seed', '1', '--pieces', '100', '--games', '5')
second = run('--seed', '1', '--pieces', '100', '--games', '5')
assert first.returncode == second.returncode == 0, (first, second)
assert first.stdout == second.stdout
rows = list(csv.DictReader(io.StringIO(first.stdout)))
assert len(rows) == 5
for seed, row in enumerate(rows, 1):
    assert int(row['seed']) == seed
    pieces, lines, points = (int(row[key]) for key in ('pieces', 'lines', 'score'))
    assert 20 <= pieces <= 100 and 0 < lines <= pieces * 4 // 10 and points > 0, row
    assert int(row['level']) == min(20, 1 + lines // 10), row
    assert row['status'] in ('limit', 'game_over')
    if row['status'] == 'limit':
        assert pieces == 100
one = list(csv.DictReader(io.StringIO(run('--pieces', '1').stdout)))
assert int(one[0]['pieces']) == 1 and one[0]['status'] == 'limit'
print('PASS seeded AI games, score/line invariants, reproducibility and argument limits')
