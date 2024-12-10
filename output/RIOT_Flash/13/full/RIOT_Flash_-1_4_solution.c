#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

#define SLEEP_DURATION (1U * US_PER_SEC)

void check_esp_err(esp_err_t err, const char *action) {
    if (err != ESP_OK) {
        printf("%s failed: %s\n", action, esp_err_to_name(err));
        while (true) {
            printf("Retrying %s...\n", action);
            xtimer_sleep(1);
            err = nvs_flash_init();
            if (err == ESP_OK) break;
        }
    }
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    check_esp_err(err, "nvs_flash_init");

    nvs_handle_t my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    check_esp_err(err, "nvs_open");

    while (1) {
        uint8_t device_state = 0;
        err = nvs_get_u8(my_handle, "device_state", &device_state);
        if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
            printf("Error (%s) reading!
", esp_err_to_name(err));
        }

        device_state = !device_state;  // Toggle state
        err = nvs_set_u8(my_handle, "device_state", device_state);
        check_esp_err(err, "nvs_set_u8");

        err = nvs_commit(my_handle);
        check_esp_err(err, "nvs_commit");

        printf("Device state updated to %d\n", device_state);

        ztimer_sleep(ZTIMER_MSEC, SLEEP_DURATION);
    }

    nvs_close(my_handle);
    return 0;
}