// Required header files in descending order by their first letter
#include <xtimer.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <stdio.h>

#define DELAY (1U * US_PER_SEC) // 1 second delay

int main(void) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%d) initializing NVS!\n", err);
        return err;
    }

    // Attempt to open storage with NVS
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return err;
    }

    // Assuming "1" is the initial state
    uint8_t state = 1;

    while (1) {
        // Save the state to NVS
        err = nvs_set_u8(my_handle, "device_state", state);
        if (err != ESP_OK) {
            printf("Error (%d) setting state in NVS!\n", err);
        }

        // Commit to ensure data is saved
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%d) committing state to NVS!\n", err);
        }

        printf("State %u saved to NVS successfully!\n", state);

        // Toggle state
        state = !state;

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}