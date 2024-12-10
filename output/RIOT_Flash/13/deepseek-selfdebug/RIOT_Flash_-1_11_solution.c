
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_log.h"
#include "ztimer.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

static void store_state(int state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        // Write
        err = nvs_set_i32(my_handle, NVS_KEY, state);
        if (err != ESP_OK) {
            ESP_LOGE("NVS", "Error (%s) writing to NVS!\n", esp_err_to_name(err));
        } else {
            ESP_LOGI("NVS", "State %d written to NVS\n", state);
        }
        // Commit
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            ESP_LOGE("NVS", "Error (%s) committing to NVS!\n", esp_err_to_name(err));
        }
        // Close
        nvs_close(my_handle);
    }
}

int main(void) {
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Continuously update the state every second
    int state = 0;
    while (1) {
        store_state(state);
        state = !state; // Toggle state
        ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for 1 second
    }

    return 0;
}
