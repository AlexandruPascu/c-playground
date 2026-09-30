"""Test real command-line programs and malformed input without external modules."""
from pathlib import Path
import subprocess
import sys
import tempfile

tic, fibonacci, components = map(lambda value: str(Path(value).resolve()), sys.argv[1:])
def run(binary, args=(), data='', expected=0):
    result = subprocess.run([binary, *args], input=data, text=True, capture_output=True, timeout=8)
    assert result.returncode == expected, (binary, args, result.returncode, result.stdout, result.stderr)
    assert 'runtime error:' not in result.stderr and 'AddressSanitizer' not in result.stderr, result.stderr
    return result.stdout

assert 'grid' in run(tic, ['--help'])
assert 'Draw again!' in run(tic, data='3 0\n')
assert 'X won' in run(tic, data='1 1\nX 0 0\n')
assert 'FULL BOARD' in run(tic, data='1 2\nX 0 0\n0 1000 1000\n')
assert 'INVALID INDEX' in run(tic, data='2 2\nX -1 0\n0 0 1\n')
assert 'NOT YOUR TURN' in run(tic, data='2 1\n0 0 0\n')
for data in ['', '0 1\nX 0 0\n', '11 0\n', '-1 0\n', '3 -1\n', '3 1\n',
             '3 1\nO 0 0\n', '3 1\nX text 0\n', '3 1\nX 9999999999999999999999 0\n',
             '999999999999999999999999 0\n', '3 1\n' + 'X' * 1000 + ' 0 0\n']:
    run(tic, data=data, expected=1)
run(tic, ['unexpected'], expected=1)
run(tic, data='10 0\n')

for n, answer in [(0, 0), (1, 1), (2, 1), (10, 55), (40, 102334155 % 666013)]:
    assert f'= {answer} (' in run(fibonacci, [str(n), 'matrix'])
assert 'skipped' in run(fibonacci, ['400000000'])
for args in [['-1'], ['garbage'], ['99999999999999999999'], ['36', 'recursive'], ['3', 'unknown']]:
    run(fibonacci, args, expected=1)

with tempfile.TemporaryDirectory(prefix='c-playground-cli-') as temporary:
    grid = Path(temporary) / 'grid.txt'
    grid.write_text('3 3\n0 0 0\n0 0 1\n0 1 1\n')
    assert run(components, [str(grid)]) == 'Maximum Area: 3\n'
    for text in ['0 0\n', '1000000 1000000\n', '1 1\n', '1 1\n2\n', '1 1\n1 0\n']:
        grid.write_text(text)
        run(components, [str(grid)], expected=1)
    run(components, [str(Path(temporary) / 'missing.txt')], expected=1)
print('PASS valid inputs, former crashes, bounds, malformed tokens, and missing files')
