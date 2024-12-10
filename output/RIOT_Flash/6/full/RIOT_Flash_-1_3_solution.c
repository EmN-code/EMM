#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <string.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>

/**
 * @brief
 */
int main(void) {
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%d) initializing NVS!\n", err);
        return err;
    }

    nvs_handle_t my_handle;
    // Open
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return err;
    }

    int32_t state = 0;
    while (1) {
        // Store state
        err = nvs_set_i32(my_handle, "state", state);
        if (err != ESP_OK) {
            printf("Error (%d) writing to NVS!\n", err);
        } else {
            printf("State %ld written to NVS.\n", (long)state);
        }

        // Wait for a second
        ztimer_sleep(ZTIMER_MSEC, 1000);

        // Toggle state
        state = !state;
    }

    // Close handle, omitted as this loop runs indefinitely
    return 0;
}
