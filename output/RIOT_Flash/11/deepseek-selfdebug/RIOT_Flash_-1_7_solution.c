
#include <stdio.h>
#include <string.h>
#include <nvs.h>
#include <xtimer.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

void store_device_state(nvs_handle_t handle, int state) {
    esp_err_t err = nvs_set_i32(handle, NVS_KEY, state);
    if (err != ESP_OK) {
        printf("Error writing to NVS: %d\n", err);
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            printf("Error committing to NVS: %d\n", err);
        }
    }
}

int main(void) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %d\n", err);
        return -1;
    }

    int state = 1; // Initial state

    while (1) {
        store_device_state(my_handle, state);
        state = (state == 1) ? 0 : 1; // Toggle state
        xtimer_sleep(1); // Wait for 1 second
    }

    nvs_close(my_handle);
    return 0;
}
