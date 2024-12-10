#include "nvs_flash.h"
#include "nvs.h"
#include "xtimer.h"
#include "stdio.h"

#define STORAGE_NAMESPACE "storage"
#define KEY "device_state"

void initialize_nvs() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void write_state_to_nvs(int state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    // Write
    err = nvs_set_i32(my_handle, KEY, state);
    if (err != ESP_OK) {
        printf("Error (%s) writing state to NVS!\n", esp_err_to_name(err));
    }

    // Commit
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing updates to NVS!\n", esp_err_to_name(err));
    }

    // Close
    nvs_close(my_handle);
}

int main(void) {
    initialize_nvs();

    int state = 0;
    while (1) {
        // Simulate state change
        state = (state == 0) ? 1 : 0;

        write_state_to_nvs(state);

        // Delay for 1 second
        xtimer_sleep(1);
    }

    return 0;
}