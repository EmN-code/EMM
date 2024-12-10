#include <unistd.h>
#include <thread.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <event/thread.h>

#define STORAGE_NAMESPACE "storage"
#define SLEEP_TIME 1

static nvs_handle_t my_handle;
static atomic_bool state = ATOMIC_VAR_INIT(0);

void *thread_function(void *arg) {
    while (1) {
        // Toggle state
        atomic_store(&state, !atomic_load(&state));

        // Store the state in non-volatile storage
        nvs_set_i32(my_handle, "device_state", atomic_load(&state));
        nvs_commit(my_handle);

        // Sleep for the specified time
        sleep(SLEEP_TIME);
    }
    return NULL;
}

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS!\n", esp_err_to_name(err));
        return err;
    }

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    // Create a new thread to handle the device state update
    thread_t state_update_thread;
    thread_create(&state_update_thread, NULL, thread_function, NULL, "state_updater");

    // Main thread continues to run, other logic can be placed here
    while (1) {
        pause(); // Suspend the main thread; the state will be updated in the new thread.
    }

    // Cleanup
    nvs_close(my_handle);
    return 0;
}