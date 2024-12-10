#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <stdio.h>

#define SLEEP_TIME (1U * US_PER_SEC)

void write_state_to_nvs(int state) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      // NVS partition was truncated and needs to be erased
      ESP_ERROR_CHECK(nvs_flash_erase());
      // Retry nvs_flash_init
      ESP_ERROR_CHECK(nvs_flash_init());
    }
    ESP_ERROR_CHECK(err);

    // Open
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        printf("Writing state %d to NVS ... ", state);
        err = nvs_set_i32(my_handle, "device_state", state);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

        // Commit
        printf("Committing updates to NVS ... ");
        err = nvs_commit(my_handle);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

        // Close
        nvs_close(my_handle);
    }
}

int main(void)
{
    int state = 0;
    while(1) {
        write_state_to_nvs(state);
        state = !state;  // Toggle state for demonstration purposes
        xtimer_sleep(1);
    }
    return 0;
}