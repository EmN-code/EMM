#include <xtimer.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>

#define STORAGE_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static void initialize_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS!\n", esp_err_to_name(err));
    }
}

static void write_state_to_nvs(uint8_t state) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &handle);
    if (err == ESP_OK) {
        err = nvs_set_u8(handle, DEVICE_STATE_KEY, state);
        if (err == ESP_OK) {
            err = nvs_commit(handle);
            if (err != ESP_OK) {
                printf("Error (%s) committing NVS!\n", esp_err_to_name(err));
            }
        } else {
            printf("Error (%s) setting value!\n", esp_err_to_name(err));
        }
        nvs_close(handle);
    } else {
        printf("Error (%s) opening NVS!\n", esp_err_to_name(err));
    }
}

int main(void) {
    initialize_nvs();

    uint8_t device_state = 0;

    while (1) {
        // Let's simulate state change every second for demonstration
        device_state = !device_state;
        printf("Writing device state: %u\n", device_state);

        write_state_to_nvs(device_state);

        xtimer_sleep(1);
    }
    return 0;
}
