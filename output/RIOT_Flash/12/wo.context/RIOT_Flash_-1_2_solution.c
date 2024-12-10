#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <xtimer.h>

#define STORAGE_NAMESPACE "storage"

void update_nvs(int device_state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    ESP_ERROR_CHECK(err);

    // Write
    err = nvs_set_i32(my_handle, "device_state", device_state);
    ESP_ERROR_CHECK(err);

    // Commit written value.
    err = nvs_commit(my_handle);
    ESP_ERROR_CHECK(err);

    // Close
    nvs_close(my_handle);

    printf("Device state updated to: %ld\n", (int32_t) device_state);
}

int main(void) {
    int32_t device_state = 0;
    while (1) {
        device_state = !device_state; // Toggle state
        update_nvs(device_state);
        xtimer_sleep(1); // Sleep for 1 second
    }
    return 0;
}