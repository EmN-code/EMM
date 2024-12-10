#include <unistd.h>
#include <thread.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <event/thread.h>

#define STORAGE_NAMESPACE "storage"

static char nvs_thread_stack[THREAD_STACKSIZE_MAIN];

void *nvs_worker(void *arg) {
    (void)arg;  // To avoid unused parameter warning
    nvs_handle_t my_handle;
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error initializing NVS\n");
        return NULL;
    }
    
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle\n");
        return NULL;
    }

    int state = 0;
    while (1) {
        state = 1 - state;  // Toggle state between 0 and 1

        err = nvs_set_i32(my_handle, "state", state);
        if (err != ESP_OK) {
            printf("Failed to set integer in NVS\n");
        }

        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Failed to commit changes to NVS\n");
        }

        sleep(1);  // Wait for 1 second
    }

    return NULL;
}

int main(void) {
    thread_create(nvs_thread_stack, sizeof(nvs_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  nvs_worker, NULL, "nvs_thread");

    while (1) {
        // Main loop can perform other tasks
    }

    return 0;
}