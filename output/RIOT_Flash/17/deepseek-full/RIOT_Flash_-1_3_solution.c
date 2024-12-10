
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <mbox.h>
#include <xtimer.h>
#include <ztimer.h>
#include <nvs.h>
#include <nvs_flash.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

int main(void)
{
    nvs_handle_t my_handle;
    uint8_t state = 0;
    int err;

    while (1) {
        // Initialize NVS
        err = nvs_flash_init();
        if (err != ESP_OK) {
            printf("NVS Flash init failed: %d\n", err);
            nvs_flash_erase();
            continue;
        }

        // Open NVS
        err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
        if (err != ESP_OK) {
            printf("NVS open failed: %d\n", err);
            nvs_flash_erase();
            continue;
        }

        // Toggle state
        state = !state;

        // Store state in NVS
        err = nvs_set_u8(my_handle, NVS_KEY, state);
        if (err != ESP_OK) {
            printf("NVS set failed: %d\n", err);
            nvs_close(my_handle);
            nvs_flash_erase();
            continue;
        }

        // Commit changes
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("NVS commit failed: %d\n", err);
            nvs_close(my_handle);
            nvs_flash_erase();
            continue;
        }

        // Close NVS handle
        nvs_close(my_handle);

        // Wait for 1 second
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}
