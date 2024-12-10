#include <stdio.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "timex.h"
#include "ztimer.h"

#define STORAGE_NAMESPACE "storage"
#define TEMP_KEY "temperature"

void update_device_state(int16_t new_state) {
    nvs_handle_t my_handle;
    esp_err_t err;
    
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle!\n");
        return;
    }
    
    err = nvs_set_i16(my_handle, TEMP_KEY, new_state);
    if (err != ESP_OK) {
        printf("Error setting value in NVS!\n");
        nvs_close(my_handle);
        return;
    }

    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error committing changes to NVS!\n");
    }

    nvs_close(my_handle);
}

int16_t read_device_state(void) {
    nvs_handle_t my_handle;
    esp_err_t err;
    int16_t device_state = 0;

    err = nvs_open(STORAGE_NAMESPACE, NVS_READONLY, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle for reading!\n");
        return device_state;
    }
    
    err = nvs_get_i16(my_handle, TEMP_KEY, &device_state);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        printf("Error reading value from NVS!\n");
    }

    nvs_close(my_handle);
    return device_state;
}

void update_temperature_periodically(void) {
    int16_t current_state = read_device_state();
    
    while (1) {
        // Simulate reading new temperature; toggle between 1 and 0
        current_state = (current_state == 0) ? 1 : 0;

        update_device_state(current_state);

        printf("Updated temperature state in NVS: %d\n", current_state);

        // Wait for 1 second
        ztimer_sleep(ZTIMER_SEC, 1);
    }
}

int main(void) {
    printf("Initializing NVS\n");
    if (nvs_flash_init() != ESP_OK) {
        printf("Error initializing NVS\n");
        return 1;
    }

    update_temperature_periodically();

    return 0;
}