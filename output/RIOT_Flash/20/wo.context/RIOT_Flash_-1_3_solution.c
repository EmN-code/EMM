#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <xtimer.h>
#include <string.h>

#define STORAGE_NAMESPACE "storage"
#define KEY "device_state"
#define DELAY (1U * US_PER_SEC)

int main(void) {
    esp_err_t err;
    nvs_handle_t my_handle;
    int32_t device_state = 0;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    
    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    ESP_ERROR_CHECK(err);

    while (1) {
        // Example: Simulate device state change
        device_state = device_state == 0 ? 1 : 0;
        
        // Write to NVS
        err = nvs_set_i32(my_handle, KEY, device_state);
        ESP_ERROR_CHECK(err);
        
        // Commit written value.
        err = nvs_commit(my_handle);
        ESP_ERROR_CHECK(err);

        printf("Stored device state: %d\n", device_state);

        // Simulate reading back the value
        int32_t read_device_state = 0;
        err = nvs_get_i32(my_handle, KEY, &read_device_state);
        ESP_ERROR_CHECK(err);
        printf("Read device state: %d\n", read_device_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}