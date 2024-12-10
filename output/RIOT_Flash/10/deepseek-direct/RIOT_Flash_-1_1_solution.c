
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "device_state"
#define NVS_KEY "state"

void store_state(int state) {
    nvs_flash_init();
    nvs_handle_t handle;
    nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    nvs_set_i32(handle, NVS_KEY, state);
    nvs_commit(handle);
    nvs_close(handle);
}

int main(void) {
    int state = 1; // Initial state

    while (1) {
        // Simulate temperature update logic here
        // For example, toggle the state between 1 and 0
        state = (state == 1) ? 0 : 1;

        // Store the updated state in NVS
        store_state(state);

        // Print the current state
        printf("Current state: %d\n", state);

        // Wait for 1 second before repeating
        xtimer_sleep(1);
    }

    return 0;
}
