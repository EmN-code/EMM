#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "nvs_flash.esp32.h"
#include "nvs.h"
#include "nvs_handle.h"
#include "esp_err.h"
#include "timex.h"
#include <unistd.h>  // Correct header for sleep

#define STORAGE_NAMESPACE "storage"

void nvs_example(void);

int main(void) {
    printf("Starting NVS example...\n");
    nvs_example();
    return 0;
}

void nvs_example(void) {
    esp_err_t err;
    // Init NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    int32_t state = 0;
    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    ESP_ERROR_CHECK(err);

    while (1) {
        state = (state == 0) ? 1 : 0;
        err = nvs_set_i32(my_handle, "device_state", state);
        ESP_ERROR_CHECK(err);

        err = nvs_commit(my_handle);
        ESP_ERROR_CHECK(err);

        printf("Stored state in NVS: %d\n", state);

        sleep(1);  // Sleep for 1 second
    }

    nvs_close(my_handle);
}