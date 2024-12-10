#include "nvs_flash.h"
#include "nvs.h"
#include <stdio.h>
#include <freertos/semphr.h>

int main(void) {
    esp_err_t err;
    nvs_handle_t my_handle;
    int32_t state = 0;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%d) initializing NVS!\n", err);
        return err;
    }

    // Open storage with read and write permissions
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return err;
    }

    // Read the current state
    err = nvs_get_i32(my_handle, "state", &state);
    switch (err) {
        case ESP_OK:
            printf("State = %d\n", (int)state);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("The value is not initialized yet!");
            state = 0; // Default
            break;
        default :
            printf("Error (%d) reading!", err);
            return err;
    }

    // Toggle state to simulate the state change
    state = !state;

    // Save the new state value
    err = nvs_set_i32(my_handle, "state", state);
    if (err != ESP_OK) {
        printf("Error (%d) writing!", err);
        return err;
    }

    // Commit written value
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) committing!", err);
        return err;
    }

    // Close NVS
    nvs_close(my_handle);

    printf("New state = %d\n", (int)state);

    return 0;
}