#include <nvs.h>
#include <xtimer.h>
#include <timex.h>
#include <stdio.h>

#define STORAGE_NAMESPACE "storage"
#define STORAGE_KEY "device_state"

void update_nvs_state(nvs_handle_t handle, const char *key, uint8_t state) {
    esp_err_t err;
    err = nvs_set_u8(handle, key, state);
    if (err != ESP_OK) {
        printf("Error (%d) writing to NVS!\n", err);
    }
    nvs_commit(handle);
}

void init_nvs() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

int main(void) {
    nvs_handle_t my_handle;
    esp_err_t err;
    uint8_t state = 0;

    // Initialize NVS
    init_nvs();
    printf("NVS initialized\n");

    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return err;
    }

    // Loop to update device state every second
    while (1) {
        state = !state; // Toggle state between 1 and 0
        update_nvs_state(my_handle, STORAGE_KEY, state);
        printf("Device state updated to: %u\n", state);

        // Sleep for a second
        xtimer_sleep(1);
    }

    nvs_close(my_handle);
    return 0;
}