
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

void store_state(nvs_handle_t handle, uint8_t state) {
    esp_err_t err = nvs_set_u8(handle, NVS_KEY, state);
    if (err != ESP_OK) {
        printf("Error setting NVS value: %d\n", err);
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            printf("Error committing NVS value: %d\n", err);
        }
    }
}

int main(void) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %d\n", err);
        return 1;
    }

    uint8_t state = 1;

    while (1) {
        // Toggle state
        state = (state == 1) ? 0 : 1;

        // Store the state in NVS
        store_state(my_handle, state);

        // Print the current state
        printf("Current state: %d\n", state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close the NVS handle
    nvs_close(my_handle);

    return 0;
}
