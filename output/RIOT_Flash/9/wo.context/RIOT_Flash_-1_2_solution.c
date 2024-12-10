#include "nvs_flash.h"
#include "nvs.h"
#include "i2c.h"
#include "xtimer.h"
#include "stdio.h"

#define NVS_NAMESPACE "storage"

void init_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void store_device_state(int state) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    ESP_ERROR_CHECK(err);

    err = nvs_set_i32(my_handle, "device_state", state);
    ESP_ERROR_CHECK(err);

    err = nvs_commit(my_handle);
    ESP_ERROR_CHECK(err);

    nvs_close(my_handle);
}

int read_device_state(void) {
    nvs_handle_t my_handle;
    int32_t device_state = 0;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    ESP_ERROR_CHECK(err);

    err = nvs_get_i32(my_handle, "device_state", &device_state);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        ESP_ERROR_CHECK(err);
    }

    nvs_close(my_handle);
    return device_state;
}

int main(void) {
    init_nvs();
    int current_state = read_device_state();

    while (1) {
        printf("Current Device State: %d\n", current_state);
        current_state = (current_state == 1) ? 0 : 1;  // Toggle state
        store_device_state(current_state);
        xtimer_sleep(1);
    }

    return 0;
}