
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "device_state"
#define NVS_KEY "state"

void store_state(nvs_handle_t handle, int state) {
    char state_str[2];
    sprintf(state_str, "%d", state);
    nvs_set_str(handle, NVS_KEY, state_str);
    nvs_commit(handle);
}

int main(void) {
    nvs_handle_t handle;
    int state = 0;

    // Initialize NVS
    nvs_init();

    // Open NVS namespace
    nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);

    while (1) {
        // Toggle state
        state = (state == 0) ? 1 : 0;

        // Store the state in NVS
        store_state(handle, state);

        // Print the current state
        printf("State stored: %d\n", state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(handle);

    return 0;
}