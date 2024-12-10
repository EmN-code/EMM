#include <stdio.h>           // For printf function
#include "xtimer.h"          // xtimer library for delaying execution
#include "nvs_flash.h"       // For NVS functionalities
#include "nvs.h"             // For NVS handle

#define TEMP_KEY "device_state"
#define STORAGE_NAMESPACE "storage"
#define DELAY_SEC 1

// Placeholder function to simulate retrieval of device state
typedef struct {
    nvs_handle_t handle;
} nvs_handle_t_sim;

nvs_handle_t_sim get_device_state(nvs_handle_t_sim my_handle) {
    int32_t temp;
    esp_err_t err = nvs_get_i32(my_handle.handle, TEMP_KEY, &temp);

    if (err == ESP_OK) {
        printf("Device state retrieved from NVS: %d\n", temp);
    } else {
        temp = 1;  // Assuming default state is 1
        printf("Failed to retrieve device state, using default: %d\n", temp);
    }

    return (nvs_handle_t_sim) {.handle = my_handle.handle};
}

// Placeholder function to simulate updating device state
void update_device_state(nvs_handle_t_sim my_handle, int32_t new_state) {
    esp_err_t err;
    err = nvs_set_i32(my_handle.handle, TEMP_KEY, new_state);

    if (err != ESP_OK) {
        printf("Failed to write to NVS. Error: %d\n", err);
    }

    err = nvs_commit(my_handle.handle);

    if (err != ESP_OK) {
        printf("Failed to commit changes. Error: %d\n", err);
    } else {
        printf("Successfully updated device state to: %d\n", new_state);
    }
}

int main(void) {
    esp_err_t err = nvs_flash_init();

    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        printf("NVS Flash was not initialized. Initializing...\n");
        err = nvs_flash_erase();
        err = nvs_flash_init();
    }

    nvs_handle_t_sim my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle.handle);

    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return err;
    }

    int32_t state = 0;
    while (1) {
        // Simulating updating the state every second
        state = (state == 0) ? 1 : 0; // toggles between 0 and 1
        update_device_state(my_handle, state);
        xtimer_sleep(DELAY_SEC);
    }

    return 0;
}