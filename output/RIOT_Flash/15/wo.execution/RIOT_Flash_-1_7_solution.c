#include <stdio.h>
#include <mutex.h>
#include <event/thread.h>
#include <timex.h>
#include <msg.h>
#include <xtimer.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <esp_system.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

nvs_handle_t my_handle;

void toggle_state(void *arg)
{
    int state = 0;
    while (1)
    {
        // Open NVS
        if (nvs_open("storage", NVS_READWRITE, &my_handle) == ESP_OK) {
            // Save the integer value to the key "device_state"
            nvs_set_i32(my_handle, "device_state", state);
            nvs_commit(my_handle);

            // toggle state
            state = !state;

            // Close NVS handle
            nvs_close(my_handle);
        }
        else
        {
            printf("Error opening NVS!");
        }

        xtimer_sleep(1); // Sleep for 1 second
    }
}

int main(void)
{
    // Initialize NVS
    if (nvs_flash_init() != ESP_OK) {
        printf("Failed to initialize NVS!");
        return -1;
    }

    // Create a new thread
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, toggle_state, NULL, "toggle_thread");

    return 0;
}