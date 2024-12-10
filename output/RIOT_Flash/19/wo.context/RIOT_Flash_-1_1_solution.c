#include <stdio.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "msg.h"
#include "dht.h"
#include "timex.h"
#include "ztimer.h"

static void perform_nvs_operations(void) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error initializing NVS: %d\n", err);
        return;
    }

    // Open NVS handle
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %d\n", err);
        return;
    }

    // Read previous state
    uint8_t device_state = 0;
    err = nvs_get_u8(my_handle, "device_state", &device_state);
    if (err == ESP_OK) {
        printf("Previous device state: %d\n", device_state);
    }

    // Toggle device state
    device_state = !device_state;
    printf("New device state: %d\n", device_state);

    // Write new state
    err = nvs_set_u8(my_handle, "device_state", device_state);
    if (err != ESP_OK) {
        printf("Error writing to NVS: %d\n", err);
    }

    // Commit updated value
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error committing to NVS: %d\n", err);
    }

    // Close NVS handle
    nvs_close(my_handle);
}

void main_loop(void) {
    msg_t msg_receive;

    while (1) {
        // Perform NVS operations
        perform_nvs_operations();

        // Wait 1 second
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
}

int main(void) {
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret != ESP_OK) {
        printf("Failed to initialize NVS: %d\n", ret);
        return ret;
    }

    // Start main loop
    main_loop();

    return 0;
}