#include <stdio.h>
#include <unistd.h> // For usleep function
#include <nvs.h>

static void read_device_state(void) {
    int32_t device_state;
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
    } else {
        printf("Done\n");
        err = nvs_get_i32(my_handle, "device_state", &device_state);
        if (err == ESP_OK) {
            printf("Device state = %d\n", (int)device_state);
        } else {
            printf("No previous device state found.\n");
        }
        nvs_close(my_handle);
    }
}

static void update_device_state(void) {
    int32_t device_state = 1; // Example default state
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err == ESP_OK) {
        err = nvs_set_i32(my_handle, "device_state", device_state);
        if (err != ESP_OK) {
            printf("Failed to write state to NVS!\n");
        }
        err = nvs_commit(my_handle);
        nvs_close(my_handle);
    } else {
        printf("Error (%d) opening NVS handle!\n", err);
    }
}

int main(void) {
    while (1) {
        update_device_state();
        read_device_state();
        usleep(1000000); // Sleep for 1 second
    }
    return 0;
}