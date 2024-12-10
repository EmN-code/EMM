
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <esp_nvs.h>
#include <esp_log.h>

#define NVS_NAMESPACE "device_state"
#define NVS_KEY "state"

void store_state(int state) {
    esp_err_t err;
    nvs_handle_t handle;

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    // Write state to NVS
    err = nvs_set_i32(handle, NVS_KEY, state);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Error (%s) writing to NVS!\n", esp_err_to_name(err));
    } else {
        ESP_LOGI("NVS", "State %d written to NVS", state);
    }

    // Commit written value
    err = nvs_commit(handle);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Error (%s) committing to NVS!\n", esp_err_to_name(err));
    }

    // Close NVS
    nvs_close(handle);
}

int read_state(void) {
    esp_err_t err;
    nvs_handle_t handle;
    int32_t state = -1;

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return state;
    }

    // Read state from NVS
    err = nvs_get_i32(handle, NVS_KEY, &state);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Error (%s) reading from NVS!\n", esp_err_to_name(err));
    } else {
        ESP_LOGI("NVS", "State %d read from NVS", state);
    }

    // Close NVS
    nvs_close(handle);

    return state;
}

int main(void) {
    int state = 0;

    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    while (1) {
        // Toggle state
        state = (state == 0) ? 1 : 0;

        // Store state in NVS
        store_state(state);

        // Read state from NVS to verify
        int read_state_value = read_state();
        if (read_state_value != state) {
            ESP_LOGE("NVS", "State mismatch! Expected %d, got %d", state, read_state_value);
        }

        // Wait for 1 second
        xtimer_sleep(1);
    }

    return 0;
}
