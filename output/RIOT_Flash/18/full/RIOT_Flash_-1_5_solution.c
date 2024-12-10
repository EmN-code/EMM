#include <xtimer.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>

#define STORAGE_NAMESPACE "storage"

void store_state(int32_t state) {
    nvs_handle_t my_handle;
    esp_err_t err;
    
    // Initialize NVS
    err = nvs_flash_init();
    while (err != ESP_OK) {
        printf("Error initializing NVS: %s. Retrying...\n", esp_err_to_name(err));
        xtimer_sleep(1);
        err = nvs_flash_init();
    }

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    while (err != ESP_OK) {
        printf("Error opening NVS handle: %s. Retrying...\n", esp_err_to_name(err));
        xtimer_sleep(1);
        err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    }

    // Write the state to NVS
    err = nvs_set_i32(my_handle, "state", state);
    if (err != ESP_OK) {
        printf("Failed to write state to NVS!\n");
    }

    // Commit the written value to NVS
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Failed to commit updates to NVS!\n");
    }

    // Close NVS handle
    nvs_close(my_handle);
}

int32_t read_state(void) {
    nvs_handle_t my_handle;
    esp_err_t err;
    int32_t state = 0;

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %s\n", esp_err_to_name(err));
        return -1;
    }

    // Read the state from NVS
    err = nvs_get_i32(my_handle, "state", &state);
    switch (err) {
        case ESP_OK:
            printf("State value is %ld\n", (long)state);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("The state is not initialized yet!\n");
            state = 0;  // Default state
            break;
        default:
            printf("Error (%s) reading state from NVS!\n", esp_err_to_name(err));
            break;
    }

    // Close NVS handle
    nvs_close(my_handle);
    return state;
}

int main(void) {
    while (1) {
        int32_t state = read_state();
        if (state != -1) {
            // Toggle state between 0 and 1
            state = (state == 0) ? 1 : 0;
            store_state(state);
        }
        xtimer_sleep(1);
    }
    return 0;
}