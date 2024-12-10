#include "nvs_flash.h"
#include "nvs.h"
#include "stdio.h"
#include "esp_timer.h"
#include "esp_log.h"

#define STORAGE_NAMESPACE "storage"
#define STORAGE_KEY "device_state"

static const char *TAG = "NVS Example";

void update_nvs(int device_state) {
    nvs_handle_t storage_handle;
    esp_err_t err;

    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &storage_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    ESP_LOGI(TAG, "Updating device state to %d", device_state);
    err = nvs_set_i32(storage_handle, STORAGE_KEY, device_state);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to update device state to NVS (%s)\n", esp_err_to_name(err));
    }

    err = nvs_commit(storage_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to commit updates in NVS (%s)\n", esp_err_to_name(err));
    }

    nvs_close(storage_handle);
}

void periodic_timer_callback(void* arg) {
    static int device_state = 0;
    device_state = 1 - device_state;  // Toggle between 0 and 1
    update_nvs(device_state);
}

int main(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    const esp_timer_create_args_t periodic_timer_args = {
            .callback = &periodic_timer_callback,
            .name = "periodic"};

    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 1000000));  // 1 second interval

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));  // Delay to let the timer do its job
    }
    return 0;
}