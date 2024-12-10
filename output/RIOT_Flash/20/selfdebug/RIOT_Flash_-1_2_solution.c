#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <nvs_flash.h>

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)

static char stack[STACK_SIZE];
static kernel_pid_t thread_pid;

void *thread_handler(void *arg) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error initializing NVS\n");
        return NULL;
    }

    // Open NVS
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS\n");
        return NULL;
    }

    // Write device state to NVS
    while (1) {
        int state = 1;  // Simulating a device state, would be updated by a sensor in reality
        err = nvs_set_i32(my_handle, "device_state", state);
        if (err != ESP_OK) {
            printf("Error writing to NVS\n");
        } else {
            printf("Written state %d to NVS\n", state);
        }

        // Commit written value
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error committing NVS\n");
        }

        xtimer_sleep(1); // Sleep for 1 second
    }

    // Close NVS
    nvs_close(my_handle);
    return NULL;
}

int main(void) {
    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                               0, thread_handler, NULL, "nvs_thread");

    if (thread_pid < 0) {
        printf("Error creating thread\n");
        return -1;
    }

    return 0;
}