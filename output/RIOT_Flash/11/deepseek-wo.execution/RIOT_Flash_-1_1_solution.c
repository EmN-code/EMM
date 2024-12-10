
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

void store_device_state(nvs_handle_t handle, uint8_t state) {
    esp_err_t err = nvs_set_u8(handle, NVS_KEY, state);
    if (err != ESP_OK) {
        printf("Error (%s) setting device state!\n", esp_err_to_name(err));
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing device state!\n", esp_err_to_name(err));
        }
    }
}

int main(void) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        // Simulate temperature reading and update state
        uint8_t device_state = (rand() % 2); // Randomly set state to 0 or 1
        store_device_state(my_handle, device_state);
        printf("Device state updated to: %d\n", device_state);

        // Wait for 1 second before repeating
        xtimer_sleep(1);
    }

    nvs_close(my_handle);
    return 0;
}
