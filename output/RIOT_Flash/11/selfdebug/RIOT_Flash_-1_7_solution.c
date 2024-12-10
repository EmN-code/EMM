#include <nvs_flash.h>
#include <nvs.h>
#include <stdio.h>

static nvs_handle_t my_nvs_handle;

void init_storage() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      // NVS partition was truncated and needs to be erased
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    err = nvs_open("storage", NVS_READWRITE, &my_nvs_handle);
    ESP_ERROR_CHECK(err);
}

void save_state(int state) {
    esp_err_t err = nvs_set_i32(my_nvs_handle, "device_state", state);
    ESP_ERROR_CHECK(err);

    err = nvs_commit(my_nvs_handle);
    ESP_ERROR_CHECK(err);
}

int load_state() {
    int state = 0;
    esp_err_t err = nvs_get_i32(my_nvs_handle, "device_state", &state);
    if (err != ESP_OK) {
        printf("Error (%s) reading state from NVS!\n", esp_err_to_name(err));
    }
    return state;
}

void app_main() {
    init_storage();

    while (1) {
        int current_state = load_state();
        printf("Current state: %d\n", current_state);

        // Toggle the state
        int new_state = !current_state;
        save_state(new_state);

        // Wait for one second
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}