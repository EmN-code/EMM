#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <xtimer.h>

#define STORAGE_NAMESPACE "storage"

void initialize_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void store_device_state(int state) {
    nvs_handle_t handle;
    esp_err_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        printf("Writing the current state to NVS.\n");
        err = nvs_set_i32(handle, "device_state", state);
        if (err != ESP_OK) {
            printf("Failed to write state to NVS!\n");
        }

        // Commit written value.
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            printf("Failed to commit changes to NVS!\n");
        }

        // Close
        nvs_close(handle);
    }
}

void nvs_store_task(void) {
    int state = 0;
    while (1) {
        // Toggle state between 1 and 0
        state = !state;
        store_device_state(state);
        printf("Stored state: %d\n", state);

        // Wait for 1 second
        xtimer_sleep(1);
    }
}

int main(void) {
    // Initialize NVS
    initialize_nvs();

    // Start storing task
    nvs_store_task();
    
    return 0;
}