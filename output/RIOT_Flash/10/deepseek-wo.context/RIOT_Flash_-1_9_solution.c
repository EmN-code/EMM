
#include <stdio.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

int main(void)
{
    nvs_handle_t my_handle;
    int err;
    int state = 1;  // Initial state

    // Open the NVS namespace
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != 0) {
        printf("Error opening NVS namespace: %d\n", err);
        return 1;
    }

    while (1) {
        // Toggle the state
        state = (state == 1) ? 0 : 1;

        // Store the state in NVS
        err = nvs_set_i32(my_handle, DEVICE_STATE_KEY, state);
        if (err != 0) {
            printf("Error writing to NVS: %d\n", err);
            nvs_close(my_handle);
            return 1;
        }

        // Commit the changes to NVS
        err = nvs_commit(my_handle);
        if (err != 0) {
            printf("Error committing to NVS: %d\n", err);
            nvs_close(my_handle);
            return 1;
        }

        printf("Device state updated to: %d\n", state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close the NVS handle
    nvs_close(my_handle);

    return 0;
}
