#include <xtimer.h>
#include <pthread.h>
#include <nvs_flash.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "nvs.h"

// The function to store the device state persistently every second.
void *store_device_state(void *args) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS!\n", esp_err_to_name(err));
        return NULL;
    }

    nvs_handle_t my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS!\n", esp_err_to_name(err));
        return NULL;
    }

    uint8_t device_state = 1;

    while (true) {
        // Store the device state
        err = nvs_set_u8(my_handle, "device_state", device_state);
        if (err != ESP_OK) {
            printf("Error (%s) setting device state in NVS!\n", esp_err_to_name(err));
        }

        // Commit the written value
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing changes in NVS!\n", esp_err_to_name(err));
        }

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS
    nvs_close(my_handle);

    return NULL;
}

int main(void) {
    pthread_t store_thread;

    // Create a thread to store the device state
    if (pthread_create(&store_thread, NULL, store_device_state, NULL) != 0) {
        fprintf(stderr, "Error creating thread\n");
        return EXIT_FAILURE;
    }

    // Wait for the thread to finish
    pthread_join(store_thread, NULL);

    return EXIT_SUCCESS;
}