#include <xtimer.h>
#include <stdio.h>
#include <nvs_flash.h>

#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second

static nvs_handle_t nvs_handle;
static int state = 0;

void init_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    err = nvs_open("storage", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle!");
    }
}

void store_state(int state) {
    esp_err_t err = nvs_set_i32(nvs_handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error (%d) writing to NVS!
", err);
    } else {
        printf("Successfully written state: %d to NVS.
", state);
        err = nvs_commit(nvs_handle);
        if (err != ESP_OK) {
            printf("Error (%d) committing to NVS!
", err);
        }
    }
}

void toggle_state(void) {
    // toggle state between 0 and 1
    state = !state;
    store_state(state);
}

int main(void) {
    init_nvs();
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        toggle_state();
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
    }

    // Close NVS handle (never reached in this example, 
    // but essential in a real application to free resources)
    nvs_close(nvs_handle);

    return 0;
}