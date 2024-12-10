
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <timex.h>
#include <time_units.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdatomic.h>
#include <ztimer.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <mbox.h>
#include <xtimer.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t my_handle;
static atomic_int device_state = 0;

void toggle_state(void) {
    device_state = !device_state;
}

void store_state(void) {
    int err;
    err = nvs_set_u8(my_handle, DEVICE_STATE_KEY, (uint8_t)device_state);
    if (err != ESP_OK) {
        printf("Failed to set state in NVS: %s\n", esp_err_to_name(err));
        return;
    }
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Failed to commit state to NVS: %s\n", esp_err_to_name(err));
    }
}

int main(void) {
    int err;

    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Failed to open NVS handle: %s\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        toggle_state();
        store_state();
        xtimer_sleep(1);
    }

    nvs_close(my_handle);
    return 0;
}
