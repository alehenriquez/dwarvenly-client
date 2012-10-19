#!/usr/bin/env python
import mkpy as mk
import glob
import os
import platform

root_dir = os.path.dirname(os.path.abspath(__file__))
source_dir = root_dir
build_dir = os.path.join(root_dir, 'build')
system = platform.system()

cc = 'clang'
opt_mode = '-O0 -g3 -DDEBUG'
c_mode= '-v -std=c11 -m32 -static'
c_warn_mode = '-W -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -ferror-limit=5'
if system.startswith('Windows'):
    c_warn_mode += ' -Wno-microsoft -Wno-unknown-pragmas -Wno-ignored-attributes -Wno-invalid-token-paste -Wno-int-to-pointer-cast -Wno-unused-value'
c_include_dirs = [source_dir]
c_lib_dirs = []
c_flags = ''
if system.startswith('FreeBSD'):
    c_include_dirs.append(r'/usr/include')
    c_include_dirs.append(r'/usr/local/include')
    c_libs = ['GLFW', 'GL', 'X11', 'Xrandr' 'SOIL']
elif system.startswith('Windows'):
    c_include_dirs.append(os.path.join(root_dir, 'include'))
    c_include_dirs.append(os.path.join(root_dir, 'include', 'locust'))
    c_include_dirs.append(r'C:\MinGW\include')
    c_include_dirs.append(r'C:\MinGW\lib\clang\3.2\include')
    c_lib_dirs.append(os.path.join(root_dir, 'lib'))
    c_lib_dirs.append(r'C:\MinGW\lib')
    c_libs = ['GLFW', 'GL', 'SOIL']
    c_flags += ' -D__GNUC__=4 '
c_flags += opt_mode
c_flags += ' -I'+' -I'.join(c_include_dirs)
c_flags += ' -L'+' -L'.join(c_lib_dirs)
c_flags += ' -l'+' -l'.join(c_libs)
c_flags += ' '+c_mode
c_flags += ' '+c_warn_mode
c_flags += ' -o build/client'

c_files = glob.glob(os.path.join(source_dir, '*.c'))
c_files_dest = {f: f.replace(source_dir, build_dir)+'.o' for f in c_files }

@mk.task
def rebuild():
    clean()
    compile()

@mk.task
def compile():
    for file in c_files:
        args = [cc]
        args.extend(c_flags.split())
        args.append('-o'+c_files_dest[file])
        args.append(file)
        mk.run(*args)

@mk.task
def clean():
    c_build_dirs = [os.path.dirname(f.replace(source_dir, build_dir)) for f in c_files]
    mk.rmdirs(build_dir)
    mk.mkdirs(build_dir)
    mk.mkdirs(*c_build_dirs)

if __name__=='__main__':
    mk.build()

