#include <xtimer.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>

#define STORAGE_NAMESPACE "storage"

int main(void) {
    esp_err_t err;
    nvs_handle_t my_handle;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS\n", esp_err_to_name(err));
        return err;
    }

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle\n", esp_err_to_name(err));
        return err;
    }

    int32_t device_state = 0; // Default state

    // Read Device State
    err = nvs_get_i32(my_handle, "device_state", &device_state);
    switch (err) {
        case ESP_OK:
            printf("Device state = %ld\n", device_state);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("The value is not initialized yet!\n");
            break;
        default :
            printf("Error (%s) reading!\n", esp_err_to_name(err));
    }

    // Set new Device State
    device_state = (device_state == 0) ? 1 : 0;
    err = nvs_set_i32(my_handle, "device_state", device_state);
    if (err != ESP_OK) {
        printf("Failed to write to NVS!\n");
    } else {
        printf("New device state = %ld\n", device_state);
    }

    // Commit changes
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Failed to commit NVS!\n");
    }

    // Close NVS handle
    nvs_close(my_handle);

    // Continuously update the state every second
    while (1) {
        xtimer_sleep(1);

        device_state = (device_state == 0) ? 1 : 0;
        err = nvs_set_i32(my_handle, "device_state", device_state);
        if (err == ESP_OK) {
            printf("Updated device state = %ld\n", device_state);
            nvs_commit(my_handle);
        } else {
            printf("Error updating state!\n");
        }
    }

    return 0;
}