#include "xtimer.h"
#include "nvs.h"
#include "nvs_flash.h"

#define STORAGE_NAMESPACE "storage"
#define UPDATE_INTERVAL (1U * US_PER_SEC)

static void update_and_store_temperature() {
    int32_t temp;
    esp_err_t err;
    nvs_handle_t my_handle;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%d) initializing NVS!\n", err);
        return;
    }

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return;
    }

    printf("NVS opened successfully\n");

    while (1) {
        // Generate a random temperature for demonstration
        temp = esp_random() % 100;  // Random temperature between 0 and 99
        printf("Updating temperature to: %d\n", temp);

        // Write temperature to NVS
        err = nvs_set_i32(my_handle, "temperature", temp);
        if (err != ESP_OK) {
            printf("Error (%d) writing to NVS!\n", err);
            nvs_close(my_handle);
            return;
        }

        // Commit updated value to the NVS
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%d) committing updates to NVS!\n", err);
            nvs_close(my_handle);
            return;
        }

        printf("Temperature updated and stored successfully\n");

        xtimer_sleep(1);  // Wait for 1 second
    }

    // Close NVS handle
    nvs_close(my_handle);
}

int main(void) {
    puts("Starting the ESP32 NVS temperature updater\n");
    update_and_store_temperature();
    return 0;
}