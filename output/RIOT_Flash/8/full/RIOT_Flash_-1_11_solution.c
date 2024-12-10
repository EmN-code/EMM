#include "ztimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "stddef.h"
#include "stdio.h"

#define STORAGE_NAMESPACE "storage"

void nvs_check_error(esp_err_t err, const char *message) {
    if (err != ESP_OK) {
        printf("Error (%s): %s\n", message, esp_err_to_name(err));
    }
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    nvs_check_error(err, "nvs_flash_init");

    if (err != ESP_OK) {
        err = nvs_flash_erase();
        nvs_check_error(err, "nvs_flash_erase");
        err = nvs_flash_init();
        nvs_check_error(err, "nvs_flash_init after erase");
    }

    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    nvs_check_error(err, "nvs_open");

    int state = 0;

    while (1) {
        state = !state;  // Toggle state between 0 and 1
        err = nvs_set_i32(my_handle, "device_state", state);
        nvs_check_error(err, "nvs_set_i32");

        err = nvs_commit(my_handle);
        nvs_check_error(err, "nvs_commit");

        printf("Stored device state: %d\n", state);

        ztimer_sleep(ZTIMER_USEC, 1000000U);  // Sleep for 1 second
    }

    nvs_close(my_handle);
    return 0;
}