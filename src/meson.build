c = meson.get_compiler('c')
mutekix_lib = c.find_library('mutekix', required : true)
project_include_directory = include_directories('../include')

python = find_program('python')

elf = executable('dumper',
  'main.c',
  include_directories: project_include_directory,
  name_suffix: 'elf',
  dependencies: mutekix_lib,
  install : false)

if romtool.found() and elf2bestape.found()
  romspec = custom_target('romspec',
    input: 'romspec.toml',
    output: 'romspec.rom',
    command: [romtool, 'build', '@INPUT@', '-o', '@OUTPUT@'])

  exe = custom_target('dumper-bestape',
    input: [elf, romspec],
    output: 'dumper.exe',
    command: [elf2bestape, '-o', '@OUTPUT@', '-r', '@INPUT1@', '@INPUT0@'])
  
  custom_target('dumper-package',
    input: ['../scripts/package.py', exe],
    output: 'dumper.zip',
    command: [python, '@INPUT@', '@OUTPUT@'])
endif
