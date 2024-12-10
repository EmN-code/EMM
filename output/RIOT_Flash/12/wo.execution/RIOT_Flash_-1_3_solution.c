#include "unistd.h"
#include "thread.h"
#include "stdlib.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdatomic.h"
#include "stddef.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "MQTTClient.h"
#include "kernel_defines.h"

#define STORAGE_NAMESPACE "storage"

void *update_nvs(void *arg) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
    } else {
        int16_t device_state = 0;

        // Loop to update the NVS value every second
        while (1) {
            device_state = (device_state == 0) ? 1 : 0;
            err = nvs_set_i16(my_handle, "device_state", device_state);
            if (err != ESP_OK) {
                printf("Failed to update NVS with device state!\n");
            }

            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Failed to commit updates in NVS!\n");
            }

            printf("Device state updated to: %d\n", device_state);

            sleep(1);
        }

        // Close
        nvs_close(my_handle);
    }
    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, update_nvs, NULL, "update_nvs_thread");

    while (1) {
        // Main thread loop
    }

    return 0;
}