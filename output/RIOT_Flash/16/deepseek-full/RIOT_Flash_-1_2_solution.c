
#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "xtimer.h"

#define DELAY_TIME (1 * 1000 * 1000U) // 1 second delay

int main(void)
{
    nvs_handle_t my_handle;
    esp_err_t err;
    uint8_t state = 0;
    char state_str[2];

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    while (1) {
        // Toggle state
        state = !state;
        snprintf(state_str, sizeof(state_str), "%d", state);

        // Write state to NVS
        err = nvs_set_str(my_handle, "device_state", state_str);
        if (err != ESP_OK) {
            printf("Error (%s) writing to NVS!\n", esp_err_to_name(err));
            nvs_close(my_handle);
            return err;
        }

        // Commit written value
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing to NVS!\n", esp_err_to_name(err));
            nvs_close(my_handle);
            return err;
        }

        // Print the current state
        printf("State updated to: %s\n", state_str);

        // Delay for 1 second
        xtimer_usleep(DELAY_TIME);
    }

    // Close NVS handle
    nvs_close(my_handle);
    return 0;
}
