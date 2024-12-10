#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <thread.h>
#include <time_units.h>
#include <timex.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <msg.h>

#define STORAGE_NAMESPACE "storage"

void write_device_state(void) {
    nvs_handle_t my_handle;
    esp_err_t err;
    
    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    ESP_ERROR_CHECK(err);

    // Read state
    int8_t state = 0;
    err = nvs_get_i8(my_handle, "state", &state);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        state = 0; // default value
    }

    // Toggle state
    state = (state == 0) ? 1 : 0;
    printf("Current state: %d\n", state);

    // Write
    err = nvs_set_i8(my_handle, "state", state);
    ESP_ERROR_CHECK(err);

    // Commit written value.
    err = nvs_commit(my_handle);
    ESP_ERROR_CHECK(err);

    // Close
    nvs_close(my_handle);
}

void *state_thread(void *arg) {
    (void)arg;  // Unused, avoid compiler warnings
    while (1) {
        write_device_state();
        sleep(1);  // sleep for 1 second
    }
    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_DEFAULT];
    kernel_pid_t thread_pid;

    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, state_thread, NULL, "state_thread");
    if (thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error: Failed to create thread.\n");
        return 1;
    }

    while (1) {
        // Main thread can perform other tasks or sleep
        sleep(3600);  // Continue running, or do other operations
    }
    return 0;
}
