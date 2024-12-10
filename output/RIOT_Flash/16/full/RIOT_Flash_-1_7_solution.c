#include "xtimer.h"
#include "timex.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "stdio.h"

#define STORAGE_NAMESPACE "storage"

void initialize_nvs() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void store_state(nvs_handle_t my_handle, const char *key, char state) {
    esp_err_t err;
    err = nvs_set_str(my_handle, key, &state);

    if (err != ESP_OK) {
        printf("Error (%s) storing state!\n", esp_err_to_name(err));
    } else {
        printf("State stored successfully.\n");
        nvs_commit(my_handle);
    }
}

void app_main(void) {
    nvs_handle_t my_handle;
    esp_err_t err;

    initialize_nvs();

    while (1) {
        err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
        if (err != ESP_OK) {
            printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
            xtimer_sleep(1);
            continue;
        }

        // Dummy value to demonstrate change
        char state = '1'; // Could toggle between '0' and '1'

        store_state(my_handle, "device_state", state);

        nvs_close(my_handle);

        xtimer_sleep(1);
    }
}