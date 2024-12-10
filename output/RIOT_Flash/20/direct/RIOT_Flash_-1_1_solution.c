#include "stdio.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_system.h"
#include "esp_err.h"
#include "xtimer.h"

#define STORAGE_NAMESPACE "storage"

void save_state(uint8_t state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open NVS in read/write mode
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle!\n");
        return;
    }

    // Write the value to NVS
    err = nvs_set_u8(my_handle, "state", state);
    if (err != ESP_OK) {
        printf("Error setting value in NVS!\n");
        nvs_close(my_handle);
        return;
    }

    // Commit the value to ensure it is saved
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error committing NVS value!\n");
    }

    // Close NVS handle
    nvs_close(my_handle);
}

int main(void) {
    printf("Starting NVS example to store device state...\n");

    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    uint8_t state = 0;

    while (1) {
        // Flip state to simulate temperature state change
        state = !state;
        printf("Saving state: %d\n", state);
        save_state(state);

        // Sleep for 1 second
        xtimer_sleep(1);
    }

    return 0;
}