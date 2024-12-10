#include <xtimer.h>
#include <nvs_flash.h>
#include <nvs_flash_read.h>
#include <stdio.h>

#define STORAGE_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"
#define UPDATE_INTERVAL (1LU * US_PER_SEC)

essp_err_t err;
nvs_handle_t my_handle;

static void write_device_state(uint8_t state) {
    err = nvs_set_u8(my_handle, DEVICE_STATE_KEY, state);
    // Check if write was successful
    if (err != ESP_OK) {
        printf("Error (%d) writing data to NVS!\n", err);
    } else {
        printf("State '%d' written successfully.\n", state);
        nvs_commit(my_handle);  // Commit written value.
    }
}

static uint8_t toggle_state(uint8_t state) {
    return state == 0 ? 1 : 0;
}

int main(void) {
    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("NVS Flash Init failed: (%d)\n", err);
        return err;
    }

    // Open or create the storage handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return err;
    }

    uint8_t current_state = 0;

    while (1) {
        // Write the device state to NVS
        write_device_state(current_state);


        // Toggle the state for the next write
        current_state = toggle_state(current_state);

        // Sleep for defined interval
        xtimer_usleep(UPDATE_INTERVAL);
    }

    // Close the NVS handle
    nvs_close(my_handle);
    return 0;
}
