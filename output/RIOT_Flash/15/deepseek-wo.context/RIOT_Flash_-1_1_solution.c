
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <nvs.h>
#include <esp_log.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"
#define DELAY_MS (1 * 1000 * 1000U)  // 1 second delay

static void store_state(nvs_handle_t handle, uint8_t state) {
    esp_err_t err = nvs_set_u8(handle, NVS_KEY, state);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Failed to set state: %s", esp_err_to_name(err));
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            ESP_LOGE("NVS", "Failed to commit state: %s", esp_err_to_name(err));
        }
    }
}

static uint8_t read_state(nvs_handle_t handle) {
    uint8_t state = 0;
    esp_err_t err = nvs_get_u8(handle, NVS_KEY, &state);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGE("NVS", "Failed to get state: %s", esp_err_to_name(err));
    }
    return state;
}

int main(void) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Failed to open NVS handle: %s", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        uint8_t current_state = read_state(handle);
        uint8_t new_state = (current_state == 1) ? 0 : 1;  // Toggle state
        store_state(handle, new_state);
        printf("Updated state to: %d\n", new_state);

        xtimer_usleep(DELAY_MS);  // Wait for 1 second
    }

    nvs_close(handle);
    return 0;
}
