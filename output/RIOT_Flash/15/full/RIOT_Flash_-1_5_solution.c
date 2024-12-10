#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <kernel_defines.h>

void store_device_state_nvs(int state);
int retrieve_device_state_nvs();

void store_device_state_nvs(int state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    while (err != ESP_OK) {
        err = nvs_open("storage", NVS_READWRITE, &my_handle);
        usleep(1000000);
    }

    err = nvs_set_i32(my_handle, "device_state", state);
    if (err == ESP_OK) {
        nvs_commit(my_handle);
    }
    nvs_close(my_handle);
}

int retrieve_device_state_nvs() {
    nvs_handle_t my_handle;
    esp_err_t err;
    int32_t state = 0;
    
    err = nvs_open("storage", NVS_READONLY, &my_handle);
    while (err != ESP_OK) {
        err = nvs_open("storage", NVS_READONLY, &my_handle);
        usleep(1000000);
    }

    err = nvs_get_i32(my_handle, "device_state", &state);
    if (err != ESP_OK) {
        state = 0; // Default state
    }

    nvs_close(my_handle);
    return state;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    while (err != ESP_OK) {
        err = nvs_flash_init();
        usleep(1000000);  // Retry every 1 second if initialization fails
    }

    int state = retrieve_device_state_nvs();

    while (1) {
        // Toggle the state between 0 and 1
        state = state ? 0 : 1;

        // Store the state persistently
        store_device_state_nvs(state);

        // Sleep for 1 second
        usleep(1000000);
    }

    return 0;
}