
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

void store_device_state(nvs_handle_t handle, uint8_t state) {
    esp_err_t err = nvs_set_u8(handle, NVS_KEY, state);
    if (err != ESP_OK) {
        printf("Error (%s) writing to NVS!\n", esp_err_to_name(err));
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing to NVS!\n", esp_err_to_name(err));
        }
    }
}

int main(void) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return 1;
    }

    uint8_t state = 0;
    while (1) {
        store_device_state(my_handle, state);
        state = (state == 0) ? 1 : 0; // Toggle state
        xtimer_sleep(1); // Wait for 1 second
    }

    nvs_close(my_handle);
    return 0;
}
