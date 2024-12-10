#include <thread.h>
#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "nvs.h"
#include "nvs_flash.h" // Ensure correct order of includes

#define STORE_PARTITION "storage"
#define STATE_KEY "device_state"

static nvs_handle_t my_handle;

void setup_nvs(void) {
    esp_err_t err = nvs_open(STORE_PARTITION, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle!");
        return;
    }
    printf("NVS handle opened.");
}

void store_state(const char *state) {
    esp_err_t err = nvs_set_str(my_handle, STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error setting state!");
    } else {
        nvs_commit(my_handle);
        printf("State stored: %s\n", state);
    }
}

void read_state(char *state, size_t size) {
    esp_err_t err = nvs_get_str(my_handle, STATE_KEY, state, &size);
    if (err == ESP_OK) {
        printf("State read: %s\n", state);
    } else {
        printf("Error reading state!");
    }
}

void update_device_state(void) {
    char state[2] = "0";
    while (1) {
        read_state(state, sizeof(state));
        state[0] = (state[0] == '0') ? '1' : '0';
        store_state(state);
        xtimer_sleep(1);
    }
}

int main(void) {
    setup_nvs();
    update_device_state();
    nvs_close(my_handle);
    return 0;
}