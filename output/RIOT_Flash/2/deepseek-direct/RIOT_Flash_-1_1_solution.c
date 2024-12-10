
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <esp_nvs.h>

#define NVS_NAMESPACE "device_state"
#define NVS_KEY "state"

void store_state(int state) {
    esp_err_t err;
    nvs_handle_t handle;

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    // Write state to NVS
    err = nvs_set_i32(handle, NVS_KEY, state);
    if (err != ESP_OK) {
        printf("Error (%s) writing to NVS!\n", esp_err_to_name(err));
    } else {
        printf("State %d stored in NVS\n", state);
    }

    // Commit changes
    err = nvs_commit(handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing to NVS!\n", esp_err_to_name(err));
    }

    // Close NVS
    nvs_close(handle);
}

int main(void) {
    int state = 1; // Initial state

    while (1) {
        // Toggle state
        state = (state == 1) ? 0 : 1;

        // Store the state in NVS
        store_state(state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    return 0;
}
