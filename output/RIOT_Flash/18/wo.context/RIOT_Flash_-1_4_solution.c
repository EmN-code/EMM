#include <unistd.h>  
#include <stdio.h>
#include "nvs_flash.h"
#include "nvs.h"

#define DELAY_SEC 1

void store_device_state(nvs_handle_t handle, int state) {
    nvs_set_i32(handle, "device_state", state);
    nvs_commit(handle);
}

int main() {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error initializing NVS Flash: %d\n", err);
        return err;
    }

    nvs_handle_t my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %d\n", err);
        return err;
    }

    int state = 0;
    while(1) {
        store_device_state(my_handle, state);
        printf("Stored device state: %d\n", state);
        state = !state;  // Toggle state between 0 and 1
        usleep(DELAY_SEC * 1000000);  // Sleep for 1 second
    }

    nvs_close(my_handle);
    return 0;
}