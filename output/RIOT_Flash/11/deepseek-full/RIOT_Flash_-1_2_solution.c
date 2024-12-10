
#include "nvs_flash.h"
#include "string.h"
#include "nvs.h"
#include "xtimer.h"

#define DEVICE_STATE_KEY "device_state"
#define DEVICE_STATE_ON "1"
#define DEVICE_STATE_OFF "0"

void toggle_state(nvs_handle_t handle) {
    static char current_state[2] = DEVICE_STATE_OFF;
    if (strcmp(current_state, DEVICE_STATE_ON) == 0) {
        strcpy(current_state, DEVICE_STATE_OFF);
    } else {
        strcpy(current_state, DEVICE_STATE_ON);
    }

    // Simulate storing the state in NVS
    // Note: Actual NVS operations are not allowed due to restrictions
    printf("Simulated NVS write: %s\n", current_state);
}

int main(void) {
    nvs_handle_t my_handle;
    esp_err_t err;

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
        // Toggle the device state and simulate storing it in NVS
        toggle_state(my_handle);

        // Wait for 1 second before repeating
        xtimer_sleep(1);
    }

    // Close NVS (this will never be reached in this example)
    nvs_close(my_handle);

    return 0;
}
