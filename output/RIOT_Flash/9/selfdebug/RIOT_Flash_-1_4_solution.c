#include <stdio.h>
#include "nvs_handle_t"
#include "nvs_flash_secure_init_partition"

#define STORAGE_NAMESPACE "storage"

void init_nvs() {
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
    ESP_ERROR_CHECK(err);

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
    } else {
        printf("Opened NVS handle successfully\n");
        nvs_close(my_handle);
    }
}

void store_device_state(int state) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) return;

    err = nvs_set_i32(my_handle, "device_state", state);
    if (err != ESP_OK) {
       printf("Failed to update device state\n");
    } else {
       printf("Device state updated successfully\n");
       nvs_commit(my_handle);
    }

    nvs_close(my_handle);
}

int main() {
    init_nvs();
    int device_state = 1;

    while (1) {
        store_device_state(device_state);
        device_state = !device_state; // toggle between 0 and 1
        usleep(1000000); // wait for 1 second
    }
    return 0;
}
