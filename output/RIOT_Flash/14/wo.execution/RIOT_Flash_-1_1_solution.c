#include <stdio.h>
#include <stdlib.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "thread.h"

#define STATE_KEY "device_state"

void save_device_state(int state) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
    } else {
        err = nvs_set_i32(my_handle, STATE_KEY, state);
        if (err != ESP_OK) {
            printf("Error (%d) writing to NVS!\n", err);
        } else {
            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Error (%d) committing to NVS!\n", err);
            }
        }
        nvs_close(my_handle);
    }
}

int read_device_state() {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    int state = 0;
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
    } else {
        err = nvs_get_i32(my_handle, STATE_KEY, &state);
        if (err != ESP_OK) {
            printf("Error (%d) reading from NVS!\n", err);
        }
        nvs_close(my_handle);
    }
    return state;
}

void *state_update_thread(void *arg) {
    while (1) {
        int state = rand() % 2;  // Randomly generate a state (0 or 1) for simulation
        printf("Updating device state to: %d\n", state);
        save_device_state(state);
        xtimer_usleep(1000000);  // Sleep for 1 second
    }
    return NULL;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%d) initializing NVS!\n", err);
        return -1;
    }

    thread_create(state_update_thread, NULL, THREAD_STACKSIZE_DEFAULT,
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  "state_update_thread");

    return 0;
}