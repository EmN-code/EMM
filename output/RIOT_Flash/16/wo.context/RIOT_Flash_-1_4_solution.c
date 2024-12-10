#include <stdio.h>
#include <stdint.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <xtimer.h>

void init_nvs(nvs_handle_t *handle) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    err = nvs_open("storage", NVS_READWRITE, handle);
    ESP_ERROR_CHECK(err);
}

void store_device_state(nvs_handle_t handle, uint32_t state) {
    esp_err_t err = nvs_set_u32(handle, "state", state);
    ESP_ERROR_CHECK(err);

    err = nvs_commit(handle);
    ESP_ERROR_CHECK(err);
}

int main(void) {
    nvs_handle_t my_handle;
    uint32_t device_state = 0;

    init_nvs(&my_handle);
    while (1) {
        device_state ^= 1; // Toggle state between 0 and 1
        printf("Storing device state: %lu\n", (unsigned long)device_state);
        store_device_state(my_handle, device_state);

        xtimer_sleep(1);
    }
    nvs_close(my_handle);
    return 0;
}