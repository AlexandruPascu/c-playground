"""Check argument failures before a display is initialized."""
import subprocess
import sys

triangle, carpet, tetris, skeleton, tic, hello = sys.argv[1:]
for executable in (triangle, carpet, tetris, skeleton, tic, hello):
    result = subprocess.run([executable, '--help'], capture_output=True, text=True, timeout=5)
    assert result.returncode == 0 and 'Usage:' in result.stdout, result
for executable, arguments in [(triangle, ['10']), (triangle, ['-1']), (triangle, ['2', '3']),
                              (carpet, ['6']), (carpet, ['garbage']),
                              (carpet, ['999999999999999999999999']),
                              (tetris, ['unexpected']), (tetris, ['--seed']),
                              (tetris, ['--seed', '0']), (tetris, ['--seed', 'bad']),
                              (tetris, ['--ai', 'bad']), (tetris, ['--weights']),
                              (tetris, ['--weights', '/missing/tetris.weights']), (skeleton, ['unexpected']),
                              (tic, ['0']), (tic, ['6']), (tic, ['bad']),
                              (tic, ['2', '3']), (hello, ['--font']),
                              (hello, ['unexpected'])]:
    result = subprocess.run([executable, *arguments], capture_output=True, text=True, timeout=5)
    assert result.returncode == 1 and result.stderr, result
print('PASS graphics help and argument validation')
