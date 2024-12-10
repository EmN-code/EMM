#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "unistd.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "arpa/inet.h"
#include "nvs.h"
#include "nvs_flash.h"

#define STORAGE_NAMESPACE "storage"
#define TEMP_KEY "temperature_key"

void initialize_nvs() {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS!\n", esp_err_to_name(err));
        nvs_flash_erase();
        nvs_flash_init();
    }
}

esp_err_t save_temperature_to_nvs(uint8_t temperature) {
    nvs_handle_t my_handle;
    esp_err_t err;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) return err;
    err = nvs_set_u8(my_handle, TEMP_KEY, temperature);
    if (err != ESP_OK) {
        nvs_close(my_handle);
        return err;
    }
    err = nvs_commit(my_handle);
    nvs_close(my_handle);
    return err;
}

uint8_t read_temperature_from_nvs() {
    nvs_handle_t my_handle;
    esp_err_t err;
    uint8_t temperature = 0;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READONLY, &my_handle);
    if (err == ESP_OK) {
        nvs_get_u8(my_handle, TEMP_KEY, &temperature);
        nvs_close(my_handle);
    }
    return temperature;
}

void update_temperature() {
    static uint8_t temperature = 0;
    temperature = (temperature == 0) ? 1 : 0;
    esp_err_t err = save_temperature_to_nvs(temperature);
    if (err != ESP_OK) {
        printf("Failed to save temperature: %s\n", esp_err_to_name(err));
    } else {
        printf("Temperature %d saved successfully.\n", temperature);
    }
}

int main() {
    initialize_nvs();

    while (1) {
        update_temperature();
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}
