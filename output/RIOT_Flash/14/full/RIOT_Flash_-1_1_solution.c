#include "xtimer.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <stdio.h>

#define STORAGE_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

void check_esp_err(esp_err_t err) {
    if (err != ESP_OK) {
        printf("NVS Error: %s\n", esp_err_to_name(err));
    }
}

void store_device_state(const char* state) {
    nvs_handle_t my_handle;
    esp_err_t err;
    
    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    check_esp_err(err);
    if (err != ESP_OK) return;
    
    // Write
    err = nvs_set_str(my_handle, DEVICE_STATE_KEY, state);
    check_esp_err(err);
    
    // Commit written value
    err = nvs_commit(my_handle);
    check_esp_err(err);

    // Close
    nvs_close(my_handle);
}

const char* update_device_state(int temp) {
    if (temp > 25) { // example condition for temperature, like a sensor
        return "1";   // "1" indicates hot
    }
    return "0"; // "0" indicates cold
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    check_esp_err(err);

    if (err != ESP_OK && err != ESP_ERR_NVS_NO_FREE_PAGES && err != ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
        check_esp_err(err);
    }

    int temp = 0;

    while(1) {
        const char* state = update_device_state(temp);
        store_device_state(state);

        printf("Stored device state: %s\n", state);

        // Increment temperature for testing, or use actual sensor readings
        temp++;
        if(temp > 30) temp = 0; // reset after certain range

        // Sleep for 1 second
        xtimer_sleep(1);
    }

    return 0;
}