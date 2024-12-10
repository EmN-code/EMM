#include <thread.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <xtimer.h>

#define STORAGE_NAMESPACE "storage"
#define KEY "temperature"
#define INTERVAL 1U * US_PER_SEC // 1-second interval

void update_temperature(nvs_handle_t my_handle, int32_t temp) {
    esp_err_t err;
    err = nvs_set_i32(my_handle, KEY, temp);
    if (err != ESP_OK) {
        printf("Error (%s) saving temperature!\n", esp_err_to_name(err));
    }
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing to storage!\n", esp_err_to_name(err));
    }
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        printf("Erasing and Initializing the flash...");
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS!\n", esp_err_to_name(err));
        return -1;
    }

    int32_t temp = 0;
    while(1) {
        update_temperature(my_handle, temp);
        printf("Temperature updated to: %ld\n", (long int)temp);
        temp = (temp == 0) ? 1 : 0; // Toggle between 0 and 1
        xtimer_sleep(INTERVAL);
    }

    nvs_close(my_handle);
    return 0;
}