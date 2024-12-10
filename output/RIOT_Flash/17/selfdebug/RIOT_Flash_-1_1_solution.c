#include "xtimer.h"
#include "timex.h"
#include "periph/rtc.h"
#include "nvs_flash.h"

void nvs_write_state(int state) {
    nvs_handle_t handle;
    esp_err_t err;
    
    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }
    
    // Open storage with defined namespace
    err = nvs_open("storage", NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    // Write the state
    err = nvs_set_i32(handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Failed to write state to NVS!\n");
    }

    // Commit to save changes
    err = nvs_commit(handle);
    if (err != ESP_OK) {
        printf("Failed to commit changes to NVS!\n");
    }

    // Close NVS
    nvs_close(handle);
}

int main(void) {
    // Initialize the NVS
    esp_err_t ret = nvs_flash_init();
    if (ret != ESP_OK) {
        printf("Failed to initialize NVS: %s\n", esp_err_to_name(ret));
        return 1;
    }

    int state = 0;

    while (1) {
        // Toggle state
        state = !state;

        // Write to NVS
        nvs_write_state(state);

        // Print to console
        printf("State saved: %d\n", state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    return 0;
}