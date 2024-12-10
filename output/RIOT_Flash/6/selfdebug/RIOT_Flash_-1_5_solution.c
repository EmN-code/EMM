#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xtimer.h"  // Includes timer functions
#include "nvs_flash.h"  // NVS APIs
#include "nvs.h"

#define STORAGE_NAMESPACE "storage"
#define KEY "device_state"

void update_device_state(){
    nvs_handle_t my_handle;
    esp_err_t err;
    
    // Initialize and open NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      // NVS partition was truncated and needs to be erased
      // Retry nvs_flash_init
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    printf("NVS partition opened and initialized successfully\n");

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        // Read the current device state
        int8_t device_state = 0; // default state
        err = nvs_get_i8(my_handle, KEY, &device_state);
        switch (err) {
            case ESP_OK:
                printf("Current state = %d\n", device_state);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("Device state not initialized yet!\n");
                break;
            default :
                printf("Error (%s) reading!");
        }

        // Toggle state
        device_state = !device_state;

        // Write the updated state
        err = nvs_set_i8(my_handle, KEY, device_state);
        if (err != ESP_OK) {
            printf("Error (%s) writing!");
        }

        // Commit written value
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Commit failed (%s)!\n", esp_err_to_name(err));
        }

        // Close handle and delay
        nvs_close(my_handle);
        printf("State updated to = %d\n", device_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }
}

int main(void)
{
    while (1) {
        update_device_state();
    }
    return 0;
}