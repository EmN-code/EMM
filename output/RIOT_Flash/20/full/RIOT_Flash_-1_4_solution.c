#include <xtimer.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>

#define SLEEP_INTERVAL_USEC (1000000U) // 1 second

void write_state_to_nvs(nvs_handle_t handle, int32_t state) {
    esp_err_t err;
    while (1) {
        err = nvs_set_i32(handle, "device_state", state);
        if (err != ESP_OK) {
            printf("Error writing to NVS!");
            xtimer_usleep(SLEEP_INTERVAL_USEC);
            continue;
        }

        err = nvs_commit(handle);
        if (err != ESP_OK) {
            printf("Error committing to NVS!");
            xtimer_usleep(SLEEP_INTERVAL_USEC);
            continue;
        }
        break;
    }
}

void app_main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    nvs_handle_t my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    ESP_ERROR_CHECK( err );

    int32_t state = 1;

    while (1) {
        write_state_to_nvs(my_handle, state);
        state = (state == 1) ? 0 : 1;  // Toggle state
        xtimer_sleep(1);
    }

    nvs_close(my_handle);
}

int main(void) {
    app_main();
    return 0;
}