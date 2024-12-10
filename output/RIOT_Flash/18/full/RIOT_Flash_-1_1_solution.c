#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <xtimer.h>

#define DEVICE_STATE_KEY "device_state"
#define NVS_NAMESPACE "storage"

void store_device_state(int state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle!");
        return;
    }

    // Write
    err = nvs_set_i32(my_handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error writing state to NVS!");
    } else {
        printf("Stored state %d to NVS.\n", state);
    }

    // Commit written value.
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error committing state to NVS!");
    }

    // Close
    nvs_close(my_handle);
}

void toggle_device_state() {
    static int state = 0;
    state = !state; // Toggle state between 0 and 1
    store_device_state(state);
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Failed to init NVS!\n");
        return 1;
    }

    // Continuously toggle and store state every second
    while (1) {
        toggle_device_state();
        xtimer_sleep(1);
    }

    return 0;
}