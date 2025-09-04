# benesta-dumper

Internal SD card dumper for Benesse x Besta devices.

Note that this is not a generic Besta NAND dumper and will not work on other Besta devices due to the use of Benesse-specific syscalls.

## Usage

Extract all the content inside the `dumper.zip` as-is to the **root directory** of an empty SD card that has more space than the internal SD card. Do NOT put them in subdirectories. Then, insert the SD card into your Benesse device and wait for the dumping process to finish. Finally, press the HOME button, remove the SD card, and merge all the files that are named `dump.bin.xxx` on a PC, following the order of the 3 digit numbers at the end of the filenames.

## Building

```sh
meson setup --buildtype=release --cross-file=/path/to/besta-rtos/meson-cross/arm-none-bestaeabi.ini builddir
meson compile -Cbuild
```
