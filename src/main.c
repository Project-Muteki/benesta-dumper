#include <stdlib.h>

#include <muteki/memory.h>
#include <muteki/utils.h>
#include <muteki/ui/event.h>
#include <mutekix/console.h>

// Declaration here

// End of declaration

int main(void) {
    ui_event_t uievent = {0};

    mutekix_console_init(NULL);
    atexit(&mutekix_console_fini);

    mutekix_console_printf("GetFreeMemory() -> %u\n", GetFreeMemory());

    // Experiment here
    mutekix_console_puts("Hello world!");
    // End of experiment

    mutekix_console_printf("GetFreeMemory() -> %u\n", GetFreeMemory());

    while (true) {
        if (
            (TestPendEvent(&uievent) || TestKeyEvent(&uievent)) &&
            GetEvent(&uievent) &&
            uievent.event_type == UI_EVENT_TYPE_KEY &&
            uievent.key_code0 == KEY_ESC
        ) {
            return 0;
        }
    }
}
