#include <stdio.h>
#include <nvs_flash.h>
#include <msg.h>
#include <time.h>

void write_device_state(uint8_t state) {
    nvs_handle_t handle;
    esp_err_t err;

    // Open NVS with a specific namespace "storage"
    err = nvs_open("storage", NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return;
    }

    // Write the state to NVS
    err = nvs_set_u8(handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error (%d) writing to NVS!\n", err);
    }

    // Commit written value
    err = nvs_commit(handle);
    if (err != ESP_OK) {
        printf("Error (%d) committing to NVS!\n", err);
    }

    // Close NVS handle
    nvs_close(handle);
}

void *state_update_thread(void *arg) {
    (void)arg;
    uint8_t device_state = 0; // Initial state
    while (1) {
        // Toggle state for example purpose
        device_state = !device_state;
        write_device_state(device_state);
        printf("Device state updated to: %u\n", device_state);
        thread_sleep(); // Sleep for one second
    }
    return NULL;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%d) initializing NVS!\n", err);
        return -1;
    }
    
    // Create a thread to periodically update the NVS state
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, state_update_thread, NULL, "state_update");
    
    // Main loop
    while (1) {
        thread_sleep();
    }
    return 0;
}