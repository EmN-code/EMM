
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <esp_nvs.h>
#include <esp_log.h>

#define NVS_NAMESPACE "device_state"
#define NVS_KEY "state"

void update_device_state(int state) {
    esp_err_t err;
    nvs_handle_t nvs_handle;

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    // Write state to NVS
    err = nvs_set_i32(nvs_handle, NVS_KEY, state);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Error (%s) writing to NVS!\n", esp_err_to_name(err));
    } else {
        ESP_LOGI("NVS", "State %d written to NVS", state);
    }

    // Commit changes
    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Error (%s) committing to NVS!\n", esp_err_to_name(err));
    }

    // Close NVS
    nvs_close(nvs_handle);
}

int main(void) {
    int state = 1; // Initial state

    while (1) {
        // Update the state in NVS
        update_device_state(state);

        // Toggle the state
        state = (state == 1) ? 0 : 1;

        // Wait for 1 second
        xtimer_sleep(1);
    }

    return 0;
}
