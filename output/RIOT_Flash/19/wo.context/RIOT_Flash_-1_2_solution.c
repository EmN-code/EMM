#include <stdio.h>
#include <string.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <esp_err.h>

static void init_nvs() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

static void store_device_state(nvs_handle_t handle, const char *state) {
    esp_err_t err;
    err = nvs_set_str(handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error (%s) writing!
", esp_err_to_name(err));
    }
    err = nvs_commit(handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing!
", esp_err_to_name(err));
    }
}

static void update_and_store_state() {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!
", esp_err_to_name(err));
    } else {
        // Simulate device state, alternating between "1" and "0"
        static int state = 0;
        store_device_state(my_handle, state ? "1" : "0");
        state = !state;
        nvs_close(my_handle);
    }
}

int main(void) {
    printf("Initializing NVS...");
    init_nvs();
    
    while (1) {
        printf("Updating and storing state...");
        update_and_store_state();
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 1 second
    }

    return 0;
}