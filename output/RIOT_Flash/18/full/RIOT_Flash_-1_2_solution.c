#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "xtimer.h"

#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "device_state"
#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second

void store_state(int state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS!\n", esp_err_to_name(err));
        return;
    }

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    } else {
        printf("NVS handle opened successfully\n");
    }

    // Write
    err = nvs_set_i32(my_handle, STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error (%s) writing to NVS handle!\n", esp_err_to_name(err));
    }

    // Commit
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing to NVS handle!\n", esp_err_to_name(err));
    }

    // Close
    nvs_close(my_handle);
    printf("State %d stored\n", state);
}

int main(void) {
    int state = 0;
    printf("Starting State Store Application\n");

    while (1) {
        store_state(state);
        state = (state == 0) ? 1 : 0; // Toggle state
        xtimer_sleep(1); // Wait for 1 second
    }

    return 0;
}