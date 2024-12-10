#include <stdio.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "periph/timer.h"
#include "xtimer.h"

#define STORAGE_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

void store_device_state(nvs_handle_t my_handle, int16_t state) {
    esp_err_t err;
    err = nvs_set_i16(my_handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error saving state!\n");
    }
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error committing the state!\n");
    }
}

int main(void) {
    esp_err_t err;
    nvs_handle_t my_handle;
    int16_t current_state = 0;

    printf("Initializing NVS ...\n");
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error initializing flash: %d\n", err);
        return err;
    }

    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS: %d\n", err);
        return err;
    }

    while (1) {
        printf("Storing state: %d\n", current_state);
        store_device_state(my_handle, current_state);
        current_state = (current_state == 0) ? 1 : 0; // Toggle state

        xtimer_sleep(1); // Wait for 1 second
    }

    nvs_close(my_handle);
    return 0;
}