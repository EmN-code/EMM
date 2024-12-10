#include <nvs_flash.h>
#include <nvs.h>
#include <xtimer.h>
#include <stdio.h>

#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "device_state"
#define DELAY_SEC (1U)

void init_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        ESP_ERROR_CHECK(nvs_flash_erase());
        // Retry nvs_flash_init
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void write_state_to_nvs(uint8_t state) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    ESP_ERROR_CHECK(err);

    err = nvs_set_u8(my_handle, STATE_KEY, state);
    ESP_ERROR_CHECK(err);

    err = nvs_commit(my_handle);
    ESP_ERROR_CHECK(err);

    nvs_close(my_handle);
}

int main(void) {
    init_nvs();
    uint8_t state = 0;

    while (1) {
        // Toggle state between 0 and 1
        state = !state;
        printf("Writing state: %d\n", state);
        write_state_to_nvs(state);

        xtimer_sleep(DELAY_SEC);
    }
    return 0;
}