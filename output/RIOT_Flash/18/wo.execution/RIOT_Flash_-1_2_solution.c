#include "thread.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "event/thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE 1024
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

void *update_storage(void *arg) {
    nvs_handle_t my_handle;
    size_t required_size;
    int8_t device_state = 0;

    nvs_flash_init();

    // Open non-volatile storage with a given namespace
    if (nvs_open("storage", NVS_READWRITE, &my_handle) != ESP_OK) {
        puts("Error opening NVS handle");
        return NULL;
    }

    while (1) {
        // Alternate between states 1 and 0
        device_state = !device_state;
        
        // Write the device state to NVS
        if (nvs_set_i8(my_handle, "device_state", device_state) != ESP_OK) {
            puts("Error writing to NVS");
        }

        // Commit the written value to NVS
        nvs_commit(my_handle);

        printf("Updated device state: %d\n", device_state);

        // Sleep for 1 second
        thread_sleep(1000);
    }

    // Close the NVS handle (not reached in this loop, but for proper practice)
    nvs_close(my_handle);
    return NULL;
}

int main(void) {
    unsigned char stack[STACK_SIZE];

    if (!thread_create(stack, sizeof(stack), THREAD_PRIORITY, 0, update_storage, NULL, "storage")) {
        puts("Error creating thread");
        return 1;
    }

    return 0;
}