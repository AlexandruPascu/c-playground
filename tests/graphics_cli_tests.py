"""Check argument failures before a display is initialized."""
import subprocess
import sys

triangle, carpet, tetris, skeleton = sys.argv[1:]
for executable in (triangle, carpet, tetris, skeleton):
    result = subprocess.run([executable, '--help'], capture_output=True, text=True, timeout=5)
    assert result.returncode == 0 and 'Usage:' in result.stdout, result
for executable, arguments in [(triangle, ['10']), (triangle, ['-1']), (triangle, ['2', '3']),
                              (carpet, ['6']), (carpet, ['garbage']),
                              (carpet, ['999999999999999999999999']),
                              (tetris, ['unexpected']), (skeleton, ['unexpected'])]:
    result = subprocess.run([executable, *arguments], capture_output=True, text=True, timeout=5)
    assert result.returncode == 1 and result.stderr, result
print('PASS graphics help and argument validation')
