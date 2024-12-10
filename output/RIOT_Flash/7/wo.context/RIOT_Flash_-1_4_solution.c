#include <stdio.h>
#include <stdbool.h>
#include <xtimer.h>
#include <nvs.h>
#include <nvs_flash.h>

#define STORAGE_NAMESPACE "storage"
#define TIMER_INTERVAL_SEC (1U)

static bool state = false;
static nvs_handle_t my_handle;

void update_state_in_nvs(void) {
    esp_err_t err = nvs_set_u8(my_handle, "device_state", (uint8_t)state);
    if (err != ESP_OK) {
        printf("Error (%d) storing state in NVS\n", err);
    }
    nvs_commit(my_handle);
    state = !state;
    printf("State updated to: %d\n", state);
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated
        // and needs to be erased
        ESP_ERROR_CHECK(nvs_flash_erase());
        // Retry nvs_flash_init
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle\n", err);
        return err;
    }

    while (1) {
        xtimer_sleep(TIMER_INTERVAL_SEC);
        update_state_in_nvs();
    }

    nvs_close(my_handle);
    return 0;
}
