#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "nvs.h"
#include "nvs_flash.h"

#define STORAGE_NAMESPACE "storage"

void init_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

int main(void) {
    init_nvs();

    nvs_handle_t my_handle;
    esp_err_t err;

    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    ESP_ERROR_CHECK(err);

    while (true) {
        uint8_t device_state = 0;
        err = nvs_get_u8(my_handle, "device_state", &device_state);

        if (err == ESP_ERR_NVS_NOT_FOUND) {
            printf("The state is not initialized yet!
");
        } else {
            ESP_ERROR_CHECK(err);
            device_state = !device_state;
        }

        err = nvs_set_u8(my_handle, "device_state", device_state);
        ESP_ERROR_CHECK(err);

        err = nvs_commit(my_handle);
        ESP_ERROR_CHECK(err);

        printf("Stored device state as: %d
", device_state);

        ztimer_sleep(ZTIMER_USEC, 1000000);
    }

    nvs_close(my_handle);
    return 0;
}