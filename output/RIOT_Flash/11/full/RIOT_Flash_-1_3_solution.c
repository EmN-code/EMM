#include "xtimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_system.h"

#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "device_state"
#define SLEEP_INTERVAL (1U * US_PER_SEC)

static void store_device_state(nvs_handle_t my_handle, int state) {
    char state_str[2]; // To hold "0" or "1"
    snprintf(state_str, sizeof(state_str), "%d", state);
    esp_err_t err;
    err = nvs_set_str(my_handle, STATE_KEY, state_str);
    if (err != ESP_OK) {
        printf("Error (%s) saving state!\n", esp_err_to_name(err));
        return;
    }
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Failed to commit updates (%s)\n", esp_err_to_name(err));
    }
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      // NVS library has detected that the NVS partition was truncated,
      // and needs to be erased.
      // Retry nvs_flash_init
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    int state = 0;
    while (1) {
        store_device_state(my_handle, state);
        state = !state; // toggle state between 0 and 1
        xtimer_usleep(SLEEP_INTERVAL);
    }

    nvs_close(my_handle);  // Although this line will never be reached
    return 0;
}
