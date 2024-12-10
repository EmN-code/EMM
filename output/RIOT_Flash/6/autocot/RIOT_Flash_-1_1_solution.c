#include <nvs_flash.h>
#include <nvs.h>
#include <stdio.h>
#include <thread.h>
#include <xtimer.h>

// Function to initialize NVS
void init_nvs() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        nvs_flash_erase();
        nvs_flash_init();
    }
}

// Function to store device state
void store_device_state(const char* key, uint8_t state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    // Write
    err = nvs_set_u8(my_handle, key, state);
    if (err != ESP_OK) {
        printf("Error (%s) writing device state!\n", esp_err_to_name(err));
    }

    // Commit
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing device state!\n", esp_err_to_name(err));
    }

    // Close
    nvs_close(my_handle);
}

// Function to toggle and store the device state every second
void *toggle_device_state(void *arg) {
    uint8_t state = 0;
    while (1) {
        // Toggle state
        state = (state == 0) ? 1 : 0;

        // Store the state in NVS
        store_device_state("device_state", state);

        // Sleep for 1 second
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    // Initialize NVS
    init_nvs();

    // Start the thread to toggle and store the device state
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, toggle_device_state,
                  NULL, "toggle_device_state");

    return 0;
}