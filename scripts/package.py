import pathlib
import re
import shutil
import sys
import zipfile

RE_IS_DOS83 = re.compile(
    r"^(?!(?:CON|PRN|AUX|NUL|COM[1-9]|LPT[1-9])(?:\..*)?$)"
    r"[A-Z0-9$%'\-_\@~`!\(\)\{\}\^#&]{1,8}"
    r"(?:\.[A-Z0-9$%'\-_\@~`!\(\)\{\}\^#&]{1,3})?$",
    flags=re.IGNORECASE
)

FALLBACK_NAME = 'dumper.exe'

def build_dist_package(exe_file: str, zip_file: str):
    exe_path = pathlib.Path(exe_file)
    exe_name = exe_path.name
    if RE_IS_DOS83.fullmatch(exe_name) is None:
        print('WARNING: Input file does not have a valid DOS 8.3 filename, using fallback name.')
        exe_name = FALLBACK_NAME
        
    with zipfile.ZipFile(zip_file, 'w', compression=zipfile.ZIP_DEFLATED, compresslevel=9) as out:
        with out.open('DUMMYFILE', 'w') as f:
            f.write(b'a')
        with out.open('UPDATECFG', 'w') as f:
            f.write(b'A:\\' + exe_name.encode('ascii'))
        with out.open(exe_name, 'w') as fout, open(exe_file, 'rb') as fin:
            shutil.copyfileobj(fin, fout)


if __name__ == '__main__':
    build_dist_package(sys.argv[1], sys.argv[2])
