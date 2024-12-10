#include <stdatomic.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>

void store_device_state(nvs_handle_t handle, int state) {
    esp_err_t err = nvs_set_i32(handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error writing to NVS!");
    }
    err = nvs_commit(handle);
    if (err != ESP_OK) {
        printf("Error committing to NVS!");
    }
}

int get_device_state(nvs_handle_t handle) {
    int state = 0;
    esp_err_t err = nvs_get_i32(handle, "device_state", &state);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        printf("Value not initialized yet!");
    } else if (err != ESP_OK) {
        printf("Error reading from NVS!");
    }
    return state;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("NVS Flash init failed!");
        return -1;
    }

    nvs_handle_t my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle!");
        return -1;
    }

    while (1) {
        int state = get_device_state(my_handle);
        state = !state;  // Toggle the state
        store_device_state(my_handle, state);
        printf("Device state updated to: %d\n", state);
        sleep(1);  // Pause for a second
    }

    nvs_close(my_handle);
    return 0;
}