#include <string.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <xtimer.h>

#define NVS_NAMESPACE "storage"
#define TIMER_INTERVAL (1U * 1000000U) // 1 second

void update_device_state(nvs_handle_t my_handle) {
    static int state = 0;
    esp_err_t err;

    // Toggle state
    state = !state;

    // Write the state to NVS
    err = nvs_set_i32(my_handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error (%d) writing to NVS\n", err);
    } else {
        printf("State updated to %d\n", state);
        nvs_commit(my_handle);
    }
}

int main(void) {
    esp_err_t err;
    nvs_handle_t my_handle;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS handle
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle\n", err);
        return err;
    }

    // Setup a periodic timer
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
        update_device_state(my_handle);
    }

    nvs_close(my_handle);
    return 0;
}