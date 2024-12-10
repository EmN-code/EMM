#include <xtimer.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>

#define STORAGE_NAMESPACE "storage"

static nvs_handle_t nvsHandle;

// Declare a function with a prototype
void store_device_state(char state);

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("Error initializing NVS: %d\n", err);
        return -1;
    }

    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvsHandle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %d\n", err);
        return -1;
    }

    while (1) {
        char state[2];

        // Read the current device state from NVS
        size_t required_size;
        err = nvs_get_str(nvsHandle, "device_state", NULL, &required_size);
        if (err == ESP_OK) {
            nvs_get_str(nvsHandle, "device_state", state, &required_size);
            printf("Current device state: %s\n", state);
        } else if (err == ESP_ERR_NVS_NOT_FOUND) {
            printf("Device state not found, setting to default '0'\n");
            strcpy(state, "0");
        } else {
            printf("Error reading device state: %d\n", err);
            return -1;
        }

        // Toggle device state
        state[0] = (state[0] == '0') ? '1' : '0';
        state[1] = '\0'; // Null-terminate the string

        // Store the new device state in NVS
        store_device_state(state[0]);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close the NVS handle (though this is never reached in this infinite loop)
    nvs_close(nvsHandle);
    return 0;
}

// Define the function with parameter types
void store_device_state(char state) {
    // Store the state as a string in NVS
    esp_err_t err = nvs_set_str(nvsHandle, "device_state", &state);
    if (err != ESP_OK) {
        printf("Error storing device state: %d\n", err);
    }

    // Commit the changes in NVS
    err = nvs_commit(nvsHandle);
    if (err != ESP_OK) {
        printf("Error committing device state in NVS: %d\n", err);
    }
}