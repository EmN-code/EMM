#include <stdio.h>
#include "nvs.h"
#include "dht.h"
#include "periph/rtc.h"

#define STORAGE_NAMESPACE "storage"

static dht_t dht_device;
static nvs_handle_t nvs_handle;

int main(void) {
    int32_t temperature = 0;
    int32_t humidity = 0;
    rtc_time_t time;
    rtc_get_time(&time);

    printf("Initializing DHT sensor\n");
    dht_params_t params;
    params.pin = GPIO_PIN(PA,10); // Example configuration
    params.type = DHT22;
    dht_init(&dht_device, &params);

    printf("Opening NVS\n");
    nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle);

    while (1) {
        dht_read(&dht_device, &temperature, &humidity);
        printf("Temp: %ld Hum: %ld\n", temperature, humidity);

        nvs_set_i32(nvs_handle, "temperature", temperature);
        nvs_set_i32(nvs_handle, "humidity", humidity);
        nvs_commit(nvs_handle);

        xtimer_sleep(1);
    }

    nvs_close(nvs_handle);
    return 0;
}