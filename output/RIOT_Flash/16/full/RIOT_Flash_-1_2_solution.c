#include "nvs_flash.h"
#include "nvs.h"
#include "msg.h"
#include "xtimer.h"

#define SLEEP_INTERVAL 1 // 1 second interval

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

void update_state(nvs_handle_t handle, int state) {
    esp_err_t err = nvs_set_i8(handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error (%s) setting the state!\n", esp_err_to_name(err));
        return;
    }
    err = nvs_commit(handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing the state!\n", esp_err_to_name(err));
    }
}

int get_state(nvs_handle_t handle) {
    int state = 0;
    esp_err_t err = nvs_get_i8(handle, "device_state", (int8_t *)&state);
    if (err != ESP_OK) {
        printf("Error (%s) reading the state!\n", esp_err_to_name(err));
    }
    return state;
}

int main(void) {
    nvs_handle_t handle;
    init_nvs(&handle);

    // Setup a loop to update state every second
    int state = get_state(handle);
    while (1) {
        update_state(handle, state);
        state = (state == 0) ? 1 : 0; // Toggle state
        printf("State updated to: %d\n", state);
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0; // This will actually never be reached
}