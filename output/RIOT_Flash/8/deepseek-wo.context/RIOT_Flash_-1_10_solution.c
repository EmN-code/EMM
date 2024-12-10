
#include <stdio.h>
#include <periph/nvs.h>
#include "xtimer.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

int main(void)
{
    nvs_handle_t handle;
    int err;
    int state = 1; // Initial state

    // Open the NVS namespace
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != 0) {
        printf("Error opening NVS namespace: %d\n", err);
        return 1;
    }

    while (1) {
        // Toggle the state
        state = (state == 1) ? 0 : 1;

        // Write the state to NVS
        err = nvs_set_u8(handle, NVS_KEY, state);
        if (err != 0) {
            printf("Error writing to NVS: %d\n", err);
            nvs_close(handle);
            return 1;
        }

        // Commit the changes to NVS
        err = nvs_commit(handle);
        if (err != 0) {
            printf("Error committing to NVS: %d\n", err);
            nvs_close(handle);
            return 1;
        }

        // Print the current state
        printf("Device state updated to: %d\n", state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close the NVS handle
    nvs_close(handle);

    return 0;
}
