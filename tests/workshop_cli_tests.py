"""Exercise the completed CLIs, including image pixels through the actual codecs."""
from pathlib import Path
import struct
import subprocess
import sys
import tempfile
import zlib

names = 'numbers vector_sum fps pin_lab highlight list_ops insertion_sort image_convert image_grayscale image_decode'.split()
programs = dict(zip(names, sys.argv[1:]))
def run(name, data='', args=(), expected=0):
    result = subprocess.run([programs[name], *map(str,args)], input=data, text=True, capture_output=True, timeout=30)
    assert result.returncode == expected, (name, args, result.returncode, result.stdout, result.stderr)
    assert 'runtime error:' not in result.stderr and 'AddressSanitizer' not in result.stderr, result.stderr
    return result

for name in names:
    assert 'Usage:' in run(name, args=['--help']).stdout
result = run('numbers', '1 2.5 3 4.1 2 9.134 -1')
assert result.stdout == '1\n3\n2\n' and result.stderr == '2.50\n4.10\n9.13\n', result
for value in ['nan','inf','garbage','1e999']:
    run('numbers', value, expected=1)
assert '4294967294' in run('vector_sum', '2 2147483647 2147483647').stdout
assert 'Sum is 0.' in run('vector_sum', '0').stdout
for value in ['-1','1000001','2 1','2 1 text']:
    run('vector_sum', value, expected=1)
for data, expected in [('6 .5 .3 .6 .2 .8 .1','Fps array: 3 3 2\nAverage fps: 2'),
                       ('6 .7 1 .1 3 .2 .5','Fps array: 2 3 1 1 2 1\nAverage fps: 1'),
                       ('2 .5 .5','Fps array: 2\nAverage fps: 2'),
                       ('1 3','Fps array: 1 1 1\nAverage fps: 1'),('0','Fps array:\nAverage fps: 0')]:
    assert run('fps',data).stdout.strip() == expected
for value in ['1 0','1 0.00000000001','1 -1','1 nan','1 1000001','-1','2 .5']:
    run('fps',value,expected=1)
assert run('highlight','Ana are mere. Merele sunt dulci. panamere.\nmere\n').stdout == 'Ana are MERE. MEREle sunt dulci. panaMERE.\n'
assert run('highlight','ababa\naba\n').stdout == 'ABABA\n'
run('highlight','x'*101+'\na\n',expected=1)
assert "Razvan's pin is 4903" in run('pin_lab',args=['--recover']).stdout
assert "Laur's balance is 100 bitcoin." in run('pin_lab','Laur 1234 see_balance exit exit').stdout
assert 'Incorrect PIN!' in run('pin_lab','Laur 1235 exit').stdout
run('pin_lab','Laur',expected=1)
assert run('insertion_sort','6 8 1 3 1 0 99').stdout == '0 1 1 3 8 99\n'
assert run('insertion_sort','0').stdout == '\n'
run('insertion_sort','3 1 -1 2',expected=1)
run('insertion_sort','10001',expected=1)
output=run('list_ops','pop_back min push_front 3 push_back 9 push_front 1 print find 9 min max pop_back pop_front pop_front pop_front quit').stdout
assert '\n1 3 9\nfound\n1\n9\n9\n1\n3\nempty\n' in output
run('list_ops','push_front x',expected=1)

def png_write(path, width, height, rgba):
    def chunk(tag, data):
        return struct.pack('>I',len(data))+tag+data+struct.pack('>I',zlib.crc32(tag+data)&0xffffffff)
    rows=b''.join(b'\x00'+rgba[y*width*4:(y+1)*width*4] for y in range(height))
    path.write_bytes(b'\x89PNG\r\n\x1a\n'+chunk(b'IHDR',struct.pack('>2I5B',width,height,8,6,0,0,0))+chunk(b'IDAT',zlib.compress(rows))+chunk(b'IEND',b''))
def png_read(path):
    data=path.read_bytes(); offset=8; compressed=b''
    while offset < len(data):
        length=struct.unpack('>I',data[offset:offset+4])[0]
        tag=data[offset+4:offset+8]; payload=data[offset+8:offset+8+length]; offset+=length+12
        if tag==b'IHDR': width,height,depth,kind,*_=struct.unpack('>2I5B',payload); assert (depth,kind)==(8,6)
        if tag==b'IDAT': compressed+=payload
    raw=zlib.decompress(compressed); stride=width*4; previous=bytearray(stride); decoded=bytearray()
    for y in range(height):
        filter_=raw[y*(stride+1)]; row=bytearray(raw[y*(stride+1)+1:(y+1)*(stride+1)])
        for i in range(stride):
            a=row[i-4] if i>=4 else 0; b=previous[i]; c=previous[i-4] if i>=4 else 0
            if filter_==0: predictor=0
            elif filter_==1: predictor=a
            elif filter_==2: predictor=b
            elif filter_==3: predictor=(a+b)//2
            elif filter_==4:
                p=a+b-c; distances=[abs(p-a),abs(p-b),abs(p-c)]; predictor=[a,b,c][distances.index(min(distances))]
            else: raise AssertionError(filter_)
            row[i]=(row[i]+predictor)&255
        decoded+=row; previous=row
    return width,height,bytes(decoded)

with tempfile.TemporaryDirectory(prefix='c-playground-workshop-') as directory:
    p=Path(directory)
    rgba=bytes([255,0,0,13, 0,255,0,27, 0,0,255,0, 255,255,255,255])
    png_write(p/'input.png',2,2,rgba)
    run('image_grayscale',args=[p/'input.png',p/'average.png',p/'weighted.png'])
    for file,values in [('average.png',[85,85,85,255]),('weighted.png',[54,182,18,255])]:
        width,height,pixels=png_read(p/file)
        assert (width,height)==(2,2)
        for i,gray in enumerate(values): assert pixels[i*4:i*4+4]==bytes([gray,gray,gray,rgba[i*4+3]])
    run('image_convert',args=[p/'input.png',p/'converted.bmp'])
    run('image_decode',args=[p/'converted.bmp',p/'roundtrip.png','0'])
    assert png_read(p/'roundtrip.png')==(2,2,rgba)
    encrypted=bytearray(rgba)
    for _ in range(127):
        for i in range(1,len(encrypted)): encrypted[i]^=encrypted[i-1]
    png_write(p/'encrypted.png',2,2,encrypted)
    assert '127 XOR passes' in run('image_decode',args=[p/'encrypted.png',p/'decoded.png']).stdout
    assert png_read(p/'decoded.png')==(2,2,rgba)
    run('image_decode',args=[p/'input.png',p/'decoded.png','-1'],expected=1)
    run('image_convert',args=[p/'missing.png',p/'fail.bmp'],expected=1)
    run('image_convert',args=[p/'input.png',p/'missing-dir/out.bmp'],expected=1)
    run('image_grayscale',args=[p/'input.png',p/'same.png',p/'same.png'],expected=1)
    (p/'broken.png').write_bytes(b'not an image')
    run('image_decode',args=[p/'broken.png',p/'out.png'],expected=1)
# Also exercise the supplied JPEG samples and the full workshop XOR fixture.
for name in ('image_convert','image_grayscale','image_decode'):
    assert 'Wrote ' in run(name).stdout
print('PASS workshop CLI boundaries, examples, image pixels/alpha, BMP conversion and XOR inversion')
