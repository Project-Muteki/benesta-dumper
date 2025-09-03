c = meson.get_compiler('c')
mutekix_lib = c.find_library('mutekix', required : true)
project_include_directory = include_directories('../include')

elf = executable('Applet',
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

  custom_target('Applet-bestape',
    input: [elf, romspec],
    output: 'Applet.exe',
    command: [elf2bestape, '-o', '@OUTPUT@', '-r', '@INPUT1@', '@INPUT0@'])
endif
