#include <xtimer.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>

#define STORAGE_NAMESPACE "storage"

static void store_device_state(int state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Initialize NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return;
    }

    // Write state to NVS
    err = nvs_set_i32(my_handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error (%d) writing to NVS!\n", err);
    } else {
        // Commit changes
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%d) committing changes in NVS!\n", err);
        }
    }

    // Close NVS handle
    nvs_close(my_handle);
}

int main(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    int device_state = 0;
    while (1) {
        store_device_state(device_state);
        printf("Stored device state: %d\n", device_state);

        // Toggle the state for next update
        device_state = !device_state;

        // Wait for 1 second
        xtimer_sleep(1);
    }
    return 0;
}