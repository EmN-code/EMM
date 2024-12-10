#include <thread.h>
#include <string.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <event/thread.h>

#define STORAGE_NAMESPACE "storage"

nvs_handle_t my_handle;

void *thread_handler(void *arg) {
    int32_t temperature = 0;
    esp_err_t err;

    while (1) {
        // Update temperature value
        temperature = (temperature + 1) % 100; // Mock temperature increasing

        // Open
        err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
        if (err != ESP_OK) {
            printf("Error (%d) opening NVS handle!\n", err);
        } else {
            // Write
            err = nvs_set_i32(my_handle, "temperature", temperature);
            if (err != ESP_OK) {
                printf("Error (%d) writing temperature to NVS!\n", err);
            } else {
                // Commit written value.
                err = nvs_commit(my_handle);
                if (err != ESP_OK) {
                    printf("Error (%d) committing temperature to NVS!\n", err);
                }
            }
            // Close
            nvs_close(my_handle);
        }

        // Sleep for a second
        thread_sleep(1);
    }

    return NULL;
}

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      // NVS partition was truncated and needs to be erased
      // Retry nvs_flash_init
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    kernel_pid_t thread_pid;
    char stack[THREAD_STACKSIZE_DEFAULT];

    // Create a thread that will handle storing the temperature
    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                               CREATE_STACKTEST, thread_handler, NULL, "temperature_thread");

    // Main thread can also perform other tasks
    while (1) {
        // Main loop logic
        thread_sleep(10);  // Sleep for 10 seconds in the main loop just as a placeholder
    }

    return 0;
}