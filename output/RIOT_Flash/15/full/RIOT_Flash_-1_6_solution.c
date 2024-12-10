#include <xtimer.h>
#include <ztimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <nvs_flash.h>
#include <nvs.h>

void initialize_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        while (1) {
            fprintf(stderr, "Failed to initialize NVS, error: %s\n", esp_err_to_name(err));
            xtimer_sleep(1);
            err = nvs_flash_init();
            if (err == ESP_OK) break;
        }
    }
}

void store_device_state(uint8_t state) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        fprintf(stderr, "Error opening NVS handle!");
    }

    err = nvs_set_u8(my_handle, "device_state", state);
    if (err != ESP_OK) {
        fprintf(stderr, "Failed to write state in NVS!");
        return;
    }

    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        fprintf(stderr, "Failed to commit state in NVS!");
    }

    nvs_close(my_handle);
}

uint8_t read_device_state(void) {
    nvs_handle_t my_handle;
    uint8_t state = 0;
    esp_err_t err = nvs_open("storage", NVS_READONLY, &my_handle);
    if (err != ESP_OK) {
        fprintf(stderr, "Error opening NVS handle!");
        return state;
    }

    err = nvs_get_u8(my_handle, "device_state", &state);
    if (err != ESP_OK) {
        fprintf(stderr, "Failed to read state from NVS!");
    }

    nvs_close(my_handle);
    return state;
}

void toggle_state(uint8_t *state) {
    *state = !(*state);
}

int main(void) {
    initialize_nvs();
    uint8_t current_state = read_device_state();

    while(1) {
        store_device_state(current_state);
        printf("Current device state: %d\n", current_state);
        toggle_state(&current_state);
        xtimer_sleep(1);
    }
    return 0;
}