
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

void update_device_state(nvs_handle_t handle, int state) {
    nvs_set_i32(handle, NVS_KEY, state);
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

        // Update state in NVS
        update_device_state(handle, state);

        // Print current state
        printf("Device state updated to: %d\n", state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(handle);

    return 0;
}
