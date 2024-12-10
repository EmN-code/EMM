#include <unistd.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <thread.h>
#include <event/thread.h>

#define STACKSIZE 1024
#define PRIORITY 5

static char thread_stack[STACKSIZE];

void update_device_state(void *arg) {
    nvs_handle_t my_handle;
    esp_err_t err;
    int device_state = 0;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error initializing NVS: %d\n", err);
        return;
    }

    while (1) {
        err = nvs_open("storage", NVS_READWRITE, &my_handle);
        if (err != ESP_OK) {
            printf("Error opening NVS handle: %d\n", err);
        } else {
            // Increment device state to toggle between 0 and 1
            device_state = !device_state;
            if (nvs_set_i8(my_handle, "device_state", device_state) != ESP_OK) {
                printf("Failed to write to NVS\n");
            }

            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Failed to commit changes to NVS: %d\n", err);
            }
            nvs_close(my_handle);
        }
        sleep(1); // Wait for one second before updating again
    }
}

int main(void) {
    thread_create(thread_stack, sizeof(thread_stack), PRIORITY, THREAD_CREATE_STACKTEST, update_device_state, NULL, "device_state_thread");
    while (1) {
        // Main thread can perform other tasks
        sleep(10); // Sleep to reduce CPU usage
    }

    return 0;
}