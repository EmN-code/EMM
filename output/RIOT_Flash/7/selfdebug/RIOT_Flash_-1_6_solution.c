#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <nvs_flash.h>

#define STORAGE_NAMESPACE "storage"

static void update_temperature_nvs(nvs_handle_t my_handle, int temperature) {
    esp_err_t err;
    err = nvs_set_i32(my_handle, "temperature", temperature);
    if (err != ESP_OK) {
        printf("Error (%s) setting temperature in NVS!\n", esp_err_to_name(err));
    } else {
        nvs_commit(my_handle);
    }
}

static nvs_handle_t initialize_nvs(void) {
    nvs_flash_init();
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    return my_handle;
}

int main(void) {
    nvs_handle_t my_handle = initialize_nvs();
    if (my_handle == 0) {
        return -1;
    }

    int temperature = 0;
    while (1) {
        update_temperature_nvs(my_handle, temperature);
        temperature = (temperature + 1) % 2; // Toggle between 0 and 1
        xtimer_sleep(1);
    }
    nvs_close(my_handle);
    return 0;
}