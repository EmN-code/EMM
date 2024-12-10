#include <unistd.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>

void update_device_state(void);

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    for (;;) {
        update_device_state();
        sleep(1);  // Pause execution for 1 second
    }
    return 0;
}

void update_device_state(void) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err == ESP_OK) {
        static int state = 0;
        state = (state == 0) ? 1 : 0;  // Toggle state between 0 and 1
        err = nvs_set_i32(my_handle, "device_state", state);
        if (err == ESP_OK) {
            nvs_commit(my_handle);
        }
        nvs_close(my_handle);
    }
}