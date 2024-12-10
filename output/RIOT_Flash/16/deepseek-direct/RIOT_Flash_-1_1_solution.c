
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "device_state"
#define NVS_KEY "temperature_state"

void store_temperature_state(int state) {
    nvs_flash_init();
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        err = nvs_set_i8(handle, NVS_KEY, state);
        if (err != ESP_OK) {
            printf("Error (%s) storing temperature state!\n", esp_err_to_name(err));
        } else {
            err = nvs_commit(handle);
            if (err != ESP_OK) {
                printf("Error (%s) committing NVS handle!\n", esp_err_to_name(err));
            } else {
                printf("Temperature state stored: %d\n", state);
            }
        }
        nvs_close(handle);
    }
}

int main(void) {
    while (1) {
        // Simulate temperature state update
        int temperature_state = (rand() % 2); // Randomly set state to 0 or 1
        store_temperature_state(temperature_state);
        xtimer_sleep(1); // Wait for 1 second
    }
    return 0;
}
