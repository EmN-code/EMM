#include <thread.h>
#include <string.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <MQTTClient.h>

#define STACKSIZE 1024
#define PRIORITY 5

static char stack[STACKSIZE];

void state_update_thread(void *arg) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }

    // Open NVS handle
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
    } else {
        while(1) {
            // Update state
            int8_t device_state = 1; // Set device state to 1 or 0 accordingly
            printf("Updating NVS with device state: %d\n", device_state);

            // Write the value to NVS
            err = nvs_set_i8(my_handle, "state", device_state);
            if (err != ESP_OK) {
                printf("Error (%d) writing to NVS!\n", err);
            }

            // Commit the write
            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Error (%d) committing NVS write!\n", err);
            }

            // Sleep for one second
            thread_sleep(1);
        }

        // Close NVS handle
        nvs_close(my_handle);
    }
}

int main(void) {
    // Create a thread to update the device state
    kernel_pid_t thread_pid = thread_create(stack, STACKSIZE, PRIORITY, 0, state_update_thread, NULL, "state_update");

    if (thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating thread!\n");
        return -1;
    }

    while(1) {
        // Main thread can perform other tasks or idle
        // This example has an empty loop as the main task is handled by the thread_update_thread
    }

    return 0;
}
