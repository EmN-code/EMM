#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "nvs_flash.h"
#include "nvs.h"

#define STORAGE_NAMESPACE "storage"
#define NVS_KEY "device_state"

void store_device_state(int state) {
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
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return;
    }

    // Write state to NVS
    err = nvs_set_i32(my_handle, NVS_KEY, state);
    printf((err != ESP_OK) ? "Failed to write\n" : "Write successful\n");

    // Commit the written value to NVS
    err = nvs_commit(my_handle);
    printf((err != ESP_OK) ? "Failed to commit\n" : "Commit successful\n");

    // Close NVS handle
    nvs_close(my_handle);
}

void update_state_periodically(void) {
    while (1) {
        int device_state = rand() % 2; // Simulate state reading
        store_device_state(device_state);
        xtimer_sleep(1); // Sleep for 1 second
    }
}

int main(void) {
    printf("Starting up and storing state...");
    update_state_periodically();
    return 0;
}