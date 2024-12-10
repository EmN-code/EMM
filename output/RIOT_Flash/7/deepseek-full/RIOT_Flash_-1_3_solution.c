
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
#define NVS_KEY "device_state"

static nvs_handle_t my_handle;

void store_state(uint8_t state) {
    esp_err_t err = nvs_set_u8(my_handle, NVS_KEY, state);
    if (err != ESP_OK) {
        printf("Error (%s) storing state\n", esp_err_to_name(err));
        return;
    }
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing state\n", esp_err_to_name(err));
    }
}

uint8_t read_state(void) {
    uint8_t state = 0;
    esp_err_t err = nvs_get_u8(my_handle, NVS_KEY, &state);
    if (err != ESP_OK) {
        printf("Error (%s) reading state\n", esp_err_to_name(err));
    }
    return state;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS\n", esp_err_to_name(err));
        return -1;
    }

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        uint8_t current_state = read_state();
        uint8_t new_state = (current_state == 1) ? 0 : 1;
        store_state(new_state);
        printf("State updated to %d\n", new_state);
        xtimer_sleep(1);
    }

    nvs_close(my_handle);
    return 0;
}
