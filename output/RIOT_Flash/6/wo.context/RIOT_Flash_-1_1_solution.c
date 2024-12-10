#include <stdio.h>
#include "xtimer.h"
#include "nvs.h"
#include "nvs_flash.h"

#define STORAGE_NAMESPACE "storage"
#define KEY "device_state"

static nvs_handle_t my_handle;

void init_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    ESP_ERROR_CHECK(err);
}

void write_state_to_nvs(int state) {
    esp_err_t err = nvs_set_i32(my_handle, KEY, state);
    ESP_ERROR_CHECK(err);
    err = nvs_commit(my_handle);
    ESP_ERROR_CHECK(err);
}

int read_state_from_nvs(void) {
    int32_t state = 0;
    esp_err_t err = nvs_get_i32(my_handle, KEY, &state);
    if (err != ESP_OK) {
        printf("Error (%s) reading!
", esp_err_to_name(err));
    }
    return state;
}

void update_device_state(void) {
    static int state = 0;
    state = read_state_from_nvs();
    state = !state;  // Toggle the state
    printf("New state: %d\n", state);
    write_state_to_nvs(state);
}

int main(void) {
    init_nvs();

    while (1) {
        update_device_state();
        xtimer_sleep(1);
    }

    nvs_close(my_handle);
    return 0;
}
