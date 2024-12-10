#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "msg.h"
#include "ztimer.h"

#define STORAGE_NAMESPACE "storage"

static ztimer_t timer;
static nvs_handle_t my_handle;

void update_device_state(void) {
    int16_t temp = 0;
    // Read temperature from sensor or set a fixed value
    // For simulation purposes, we'll toggle the temp state between 1 and 0
    static uint8_t state = 0;
    state = !state;

    esp_err_t err = nvs_set_u8(my_handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error (%s) saving to NVS!\n", esp_err_to_name(err));
    }

    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Failed to commit updates in NVS!\n");
    }

    printf("Device state updated to: %d\n", state);

    ztimer_start(&timer);
}

int main(void) {
    // Initialize the NVS
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS!\n", esp_err_to_name(err));
        return -1;
    }

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return -1;
    }

    // Setup and start a ztimer
    ztimer_set(&timer, 1000, update_device_state);
    ztimer_start(&timer);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}