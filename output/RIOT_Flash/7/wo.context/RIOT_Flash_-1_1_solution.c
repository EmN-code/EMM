#include <thread.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <ztimer.h>
#include <stdio.h>
#include <stdlib.h>

#define STORAGE_NAMESPACE "storage"

static nvs_handle_t my_handle;

void init_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
    }
}

void store_device_state(int state) {
    esp_err_t err = nvs_set_i32(my_handle, "device_state", state);
    printf((err != ESP_OK) ? "Failed to write device state!\n" : "Device state written successfully!\n");
    nvs_commit(my_handle);
}

void read_device_state(void) {
    int32_t state = 0; // Assume default value is 0
    esp_err_t err = nvs_get_i32(my_handle, "device_state", &state);
    switch (err) {
        case ESP_OK:
            printf("Device state = %d\n", state);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("Device state not found\n");
            state = 0; // Initialize if not found
            store_device_state(state);
            break;
        default :
            printf("Error (%d) reading device state\n", err);
    }
}

void *update_state_thread(void *arg) {
    (void)arg;
    int state = 0;
    while (1) {
        state = (state == 0) ? 1 : 0; // Toggle state between 0 and 1
        store_device_state(state);
        ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for 1000 ms or 1 second
    }
    return NULL;
}

int main(void) {
    init_nvs();
    read_device_state();

    thread_create(NULL, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN, 0, update_state_thread, NULL, "update_state_thread");

    return 0;
}