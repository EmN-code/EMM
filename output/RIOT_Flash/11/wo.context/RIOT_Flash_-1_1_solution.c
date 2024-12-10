#include <stdio.h>
#include <stdint.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "thread.h"
#include "msg.h"

#define STORAGE_NAMESPACE "storage"

static kernel_pid_t main_thread_pid;

// Timer thread function
static void *storage_write_loop(void *arg) {
    nvs_handle_t nvs_handle;
    esp_err_t err;

    printf("Opening Non-Volatile Storage (NVS) handle...\n");
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return NULL;
    }

    int state = 0;

    while (1) {
        state = (state + 1) % 2; // Toggle state between 0 and 1
        printf("Updating state to NVS: %d\n", state);

        err = nvs_set_i32(nvs_handle, "device_state", state);
        if (err != ESP_OK) {
            printf("Failed to write to NVS (%s)!\n", esp_err_to_name(err));
        } else {
            err = nvs_commit(nvs_handle);
            if (err != ESP_OK) {
                printf("Failed to commit changes in NVS (%s)!\n", esp_err_to_name(err));
            }
        }

        // Sleep for 1 second
        thread_sleep();
    }
    nvs_close(nvs_handle);
    return NULL;
}

int main(void) {
    printf("Initializing NVS...\n");
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Failed to init NVS (%s)!\n", esp_err_to_name(err));
        return -1;
    }

    main_thread_pid = thread_create(NULL, 1024, "storage_write_loop", 
                      0, storage_write_loop, NULL);
    
    if (main_thread_pid < 0) {
        printf("Failed to create timer thread!\n");
        return -1;
    }

    // Keep main thread alive
    msg_t msg_queue[4];
    msg_init_queue(msg_queue, 4);
    while (1) {
        msg_receive(NULL);
    }
    return 0;
}