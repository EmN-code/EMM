#include <xtimer.h>
#include <stdio.h>
#include "nvs_flash.h"
#include "nvs.h"

#define STORAGE_NAMESPACE "storage"

void init_nvs()
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      // NVS partition was truncated and needs to be erased
      // Retry nvs_flash_init
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void store_state(int state)
{
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open NVS
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) return;

    // Calculate temperature and update the state
    err = nvs_set_i32(my_handle, "device_state", state);
    if (err != ESP_OK) return;

    // Commit written value.
    err = nvs_commit(my_handle);

    // Close
    nvs_close(my_handle);
}

int main(void)
{
    init_nvs();
    int current_state = 0;

    while (1) {
        printf("Storing state: %d\n", current_state);
        store_state(current_state);

        // Update the state
        current_state = !current_state;  // Toggle between 0 and 1

        // Sleep for 1 second
        xtimer_sleep(1);
    }

    return 0;
}