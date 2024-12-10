
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <xtimer.h>
#include <ztimer.h>
#include <msg.h>
#include <time_units.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <nvs.h>
#include <stddef.h>
#include <mbox.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

void store_device_state(nvs_handle_t handle, int state) {
    nvs_set_i32(handle, DEVICE_STATE_KEY, state);
    nvs_commit(handle);
}

int main(void) {
    nvs_handle_t handle;
    int state = 1; // Initial state

    // Initialize NVS
    nvs_flash_init();
    nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);

    while (1) {
        // Toggle state
        state = (state == 1) ? 0 : 1;

        // Store the state in NVS
        store_device_state(handle, state);

        // Print the current state
        printf("Device state updated to: %d\n", state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(handle);

    return 0;
}
