#include <stdlib.h>
#include "nvs_flash.h"
#include "nvs.h"
#include <stdio.h>

#define STORAGE_NAMESPACE "storage"

int main(void) {
    esp_err_t err = nvs_flash_init();
    nvs_handle_t my_handle;

    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS!\n", esp_err_to_name(err));
        return err;
    }

    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    int state = 0;
    while(1) {
        state = (state == 0) ? 1 : 0;  // Toggle state between 0 and 1

        // Store the state
        err = nvs_set_i32(my_handle, "state", state);
        if (err != ESP_OK) {
            printf("Error (%s) setting NVS state!\n", esp_err_to_name(err));
        } else {
            printf("State (%d) written to NVS\n", state);
        }

        int temperature = rand() % 100; // Simulate temperature reading
        err = nvs_set_i32(my_handle, "temperature", temperature);
        if (err != ESP_OK) {
            printf("Error (%s) setting NVS temperature!\n", esp_err_to_name(err));
        } else {
            printf("Temperature (%d) written to NVS\n", temperature);
        }

        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing updates!\n", esp_err_to_name(err));
        }

        sleep(1); // Wait for one second
    }

    nvs_close(my_handle);
    return 0;
}