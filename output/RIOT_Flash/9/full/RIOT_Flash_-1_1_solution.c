#include <nvs_flash.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define STORAGE_NAMESPACE "storage"

void check_err(esp_err_t err) {
    if (err != ESP_OK) {
        printf("Error: %s\n", esp_err_to_name(err));
    }
}

void update_state(nvs_handle_t handle) {
    uint8_t state;
    size_t required_size = sizeof(state);
    esp_err_t err = nvs_get_u8(handle, "state", &state);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        state = 0; // Start with state = 0 if not found
    } else {
        check_err(err);
    }
    
    printf("Current state: %d\n", state);
    state = !state;  // Toggle state
    err = nvs_set_u8(handle, "state", state);
    check_err(err);
    err = nvs_commit(handle);
    check_err(err);
    printf("Updated state: %d\n", state);
}

void app_main() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    check_err(err);

    nvs_handle_t handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &handle);
    check_err(err);

    while (1) {
        update_state(handle);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    nvs_close(handle);
}
