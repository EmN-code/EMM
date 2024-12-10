#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <time.h>

#define STORAGE_NAMESPACE "storage"

void store_device_state(int state) {
    nvs_handle_t my_handle;
    esp_err_t err;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return;
    }
    err = nvs_set_i32(my_handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error (%d) writing to NVS!\n", err);
    }
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) committing to NVS!\n", err);
    }
    nvs_close(my_handle);
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    
    int state = 0;
    while (true) {
        store_device_state(state);
        state = !state; // toggle state between 0 and 1
        sleep(1); // wait for 1 second
    }
    return 0;
}