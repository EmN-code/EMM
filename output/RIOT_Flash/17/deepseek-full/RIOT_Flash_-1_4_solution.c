
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <unistd.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <xtimer.h>
#include <ztimer.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <kernel_defines.h>
#include <mbox.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

int main(void)
{
    nvs_handle_t my_handle;
    int32_t state = 0;
    int err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("NVS Flash initialization failed: %d\n", err);
        return -1;
    }

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %d\n", err);
        return -1;
    }

    while (1) {
        // Read the current state from NVS
        err = nvs_get_i32(my_handle, NVS_KEY, &state);
        if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
            printf("NVS read failed: %d\n", err);
            nvs_close(my_handle);
            return -1;
        }

        // Toggle the state
        state = (state == 0) ? 1 : 0;

        // Store the new state in NVS
        err = nvs_set_i32(my_handle, NVS_KEY, state);
        if (err != ESP_OK) {
            printf("NVS write failed: %d\n", err);
            nvs_close(my_handle);
            return -1;
        }

        // Commit the changes
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("NVS commit failed: %d\n", err);
            nvs_close(my_handle);
            return -1;
        }

        // Print the current state
        printf("Current state: %ld\n", (long)state);

        // Wait for 1 second
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    // Close NVS
    nvs_close(my_handle);

    return 0;
}
