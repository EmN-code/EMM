#include "xtimer.h"
#include "stdio.h"
#include "nvs.h"

#define STORAGE_NAMESPACE "storage"
#define STORAGE_KEY "device_state"

void store_device_state(int state) {
    nvs_handle_t my_handle;
    int err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != NVS_OK) {
        printf("Error (%d) opening NVS handle!
", err);
        return;
    }

    err = nvs_set_i32(my_handle, STORAGE_KEY, state);
    if (err != NVS_OK) {
        printf("Failed to write to NVS (%d)\n", err);
    }

    err = nvs_commit(my_handle);
    if (err != NVS_OK) {
        printf("Failed to commit NVS changes (%d)\n", err);
    }

    // Close the handle after done
    nvs_close(my_handle);
}

void toggle_device_state(void) {
    static int current_state = 0;
    current_state = !current_state; // Toggle state between 1 and 0
    store_device_state(current_state);
}

int main(void) {
    // Initialize the NVS
    nvs_flash_init();

    while (1) {
        toggle_device_state();
        xtimer_sleep(1);  // Sleep for 1 second
    }

    return 0;
}