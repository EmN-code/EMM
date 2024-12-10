#include <ztimer.h>
#include <string.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <esp_system.h>

#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second interval

int main(void) {
    nvs_handle_t my_handle;
    esp_err_t err;
    
    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS handle
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return -1;
    }

    int32_t state = 0;
    while (1) {
        // Writing to NVS
        err = nvs_set_i32(my_handle, "device_state", state);
        if (err == ESP_OK) {
            printf("State %ld stored successfully!\n", (long)state);
            state = !state;  // Toggle between 0 and 1
        } else {
            printf("Failed to store state with error: %s\n", esp_err_to_name(err));
        }

        // Commit changes
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Failed to commit NVS changes: %s\n", esp_err_to_name(err));
        }

        // Sleep for the defined interval
        ztimer_sleep(TIMER_INTERVAL);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}