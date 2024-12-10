
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <periph/nvs.h>
#include <periph/temp.h>  // Include the temperature peripheral header

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"
#define NVS_VALUE_SIZE 2

void update_device_state(void) {
    static char state[NVS_VALUE_SIZE];
    static int temp;
    nvs_handle_t handle;
    int err;

    // Read the current temperature
    temp = temp_read();

    // Determine the device state based on temperature
    if (temp > 25) {
        strcpy(state, "1");
    } else {
        strcpy(state, "0");
    }

    // Open the NVS namespace
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != NVS_OK) {
        printf("Error opening NVS handle: %d\n", err);
        return;
    }

    // Write the device state to NVS
    err = nvs_set_str(handle, NVS_KEY, state);
    if (err != NVS_OK) {
        printf("Error writing to NVS: %d\n", err);
    } else {
        printf("Device state updated to: %s\n", state);
    }

    // Commit the changes
    nvs_commit(handle);

    // Close the NVS handle
    nvs_close(handle);
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        update_device_state();
        xtimer_periodic_wakeup(&last_wakeup, 1 * US_PER_SEC);
    }

    return 0;
}
