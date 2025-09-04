#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <muteki/devio.h>
#include <muteki/system.h>
#include <muteki/ui/event.h>
#include <mutekix/console.h>

#define SECTOR_SIZE (512u)
#define SECTORS_PER_MB (2048u)
#define DUMP_LIMIT_MB (1024u)

#define IOCTL_VNL_GET_INFO (0x100u)

extern int sd_read_page(unsigned int device_id, unsigned int start_sector, unsigned char *buf, size_t sector_count);
extern void LockSystem(void);
extern void UnlockSystem(void);
extern short SetAutoPowerOff(int, short);
static unsigned char buf[SECTOR_SIZE * SECTORS_PER_MB];

struct vnl_info_s {
    unsigned char unk_0x0[0x30];
    char name[0x10];
    unsigned char unk_0x40[0x8];
    size_t size_mb;
    size_t size_mb_2;
    size_t sector_size;
    size_t sector_size_2;
    unsigned char unk_0x58[0x2c];
};
typedef struct vnl_info_s vnl_info_t;

bool open_dump_segment(FILE **current_file, unsigned short index) {
    static char filename[256];
    if (current_file == NULL) {
        return false;
    }
    if (*current_file != NULL) {
        fclose(*current_file);
        *current_file = NULL;
    }
    sniprintf(filename, sizeof(filename), "A:\\dump.bin.%03u", index);
    mutekix_console_printf("Writing to %s...\n", filename);
    FILE *new_file = fopen(filename, "wb");
    if (new_file == NULL) {
        return false;
    }
    *current_file = new_file;
    return true;
}

size_t get_size_mb(void) {
    // On Pocket Challenge and other eSD/eMMC devices sending IOCTL to the VNL device seems to be the only way to read
    // out the total storage capacity
    vnl_info_t vnl_info = {0};
    devio_descriptor_t *vnl = CreateFile(
        "\\\\?\\VNL",
        0x80000000,
        1,
        NULL,
        3,
        0x80,
        NULL
    );
    if (vnl == DEVIO_DESC_INVALID) {
        mutekix_console_puts("Unable to open VNL device.");
        return 0;
    }
    if (DeviceIoControl(
        vnl,
        IOCTL_VNL_GET_INFO,
        NULL,
        0,
        &vnl_info,
        sizeof(vnl_info),
        NULL,
        NULL
    ) != 0) {
        mutekix_console_puts("WARNING: IOCTL invocation unsuccessful.");
        return 0;
    }
    CloseHandle(vnl);

    mutekix_console_printf("Card %5s is %dMB in size.\n", vnl_info.name, vnl_info.size_mb);

    return vnl_info.size_mb;
}

int main(void) {
    FILE *current_file = NULL;
    size_t next_sector = 0;
    size_t total_sectors = 0;

    ui_event_t uievent = {0};

    mutekix_console_init(NULL);
    atexit(&mutekix_console_fini);

    LockSystem();
    SetAutoPowerOff(0, 1);
    mutekix_console_puts("Benesta Dumper v1.0.0");

    total_sectors = get_size_mb() * SECTORS_PER_MB;
    if (total_sectors == 0) {
        mutekix_console_puts("Cannot detect SD card size. Press HOME button to power off.");
        goto end;
    }
    for (
        next_sector = 0;
        next_sector < total_sectors;
        next_sector += SECTORS_PER_MB
    ) {
        sd_read_page(0, next_sector, buf, SECTORS_PER_MB);
        if (next_sector % (SECTORS_PER_MB * DUMP_LIMIT_MB) == 0) {
            mutekix_console_puts("");
            if (!open_dump_segment(&current_file, next_sector / (SECTORS_PER_MB * DUMP_LIMIT_MB))) {
                mutekix_console_puts("Unable to open output file. Press HOME button to power off.");
                goto end;
            }
        }
        if (current_file == NULL) {
            goto end;
        }
        fwrite(buf, 1, sizeof(buf), current_file);
        mutekix_console_printf(".");
    }
    if (current_file != NULL) {
        fclose(current_file);
    }
    mutekix_console_printf("\nDumped %u sectors. Press HOME button to power off.\n", next_sector);

end:
    while (true) {
        if (
            (TestPendEvent(&uievent) || TestKeyEvent(&uievent)) &&
            GetEvent(&uievent) &&
            uievent.event_type == UI_EVENT_TYPE_KEY &&
            uievent.key_code0 == KEY_HOME
        ) {
            SetAutoPowerOff(1, 1);
            UnlockSystem();
            SysPowerOff();
            return 0;
        }
    }
}
