#include <xtimer.h>
#include <stdio.h>
#include <nvs_flash.h>

#define STORAGE_NAMESPACE "storage"  

void toggle_state(int new_state) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error: Failed to initialize NVS!\n");
        return;
    }
    
    // Open NVS handle
    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error: Failed to open NVS handle!\n");
        return;
    }

    // Write the new state
    err = nvs_set_i32(my_handle, "device_state", new_state);
    if (err != ESP_OK) {
        printf("Error: Failed to write to NVS!\n");
    }
    
    // Commit written value to NVS
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error: Failed to commit to NVS!\n");
    }

    // Close NVS handle
    nvs_close(my_handle);
}

void update_temperature_state(void) {
    static int state = 0;
    state = !state;  // Toggle state
    toggle_state(state);
    printf("Updated state to: %d\n", state);
}

int main(void) {
    while (1) {
        update_temperature_state();
        xtimer_sleep(1);  // Sleep for 1 second
    }
    return 0;
}