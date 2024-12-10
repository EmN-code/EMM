
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "device_state"
#define NVS_KEY "state"

void store_state(nvs_handle_t handle, int state) {
    int32_t state_to_store = state;
    nvs_set_i32(handle, NVS_KEY, state_to_store);
    nvs_commit(handle);
}

int main(void) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return 1;
    }

    int state = 1; // Initial state

    while (1) {
        // Simulate temperature update logic here
        // For example, toggle the state every second
        state = (state == 1) ? 0 : 1;

        // Store the updated state in NVS
        store_state(handle, state);

        // Print the current state
        printf("Current state: %d\n", state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close the NVS handle
    nvs_close(handle);

    return 0;
}
