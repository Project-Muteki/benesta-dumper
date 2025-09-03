# meson-project-template

Template of Besta RTOS applet using the meson build system.

## Quick start

```sh
meson setup --buildtype=release --cross-file=/path/to/besta-rtos/meson-cross/arm-none-bestaeabi.ini builddir
meson compile -Cbuild
```
