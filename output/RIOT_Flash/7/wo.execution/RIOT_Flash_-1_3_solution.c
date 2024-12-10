#include <stddef.h>
#include <stdatomic.h>
#include <time_units.h>
#include <stdio.h>
#include <unistd.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <stdbool.h>
#include <stdlib.h>
#include <kernel_defines.h>

void update_temperature_state(void) {
    // Variable declaration
    int32_t temp = 1; // you can modify this depending on your application
    nvs_handle_t my_handle;
    esp_err_t err;

    // Initialize the NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error initializing NVS\n");
        return;
    }

    // Open the NVS handle for read/write operations
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle\n");
        return;
    }

    // Set the temperature value in NVS
    err = nvs_set_i32(my_handle, "temperature", temp);
    if (err != ESP_OK) {
        printf("Error setting temperature in NVS\n");
    }

    // Commit changes to NVS
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error committing changes to NVS\n");
    }

    // Close the NVS handle
    nvs_close(my_handle);
}

int main(void) {
    // Continuously update the temperature every second
    while (true) {
        update_temperature_state();
        sleep(1); // Sleep for 1 second
    }

    return 0;
}