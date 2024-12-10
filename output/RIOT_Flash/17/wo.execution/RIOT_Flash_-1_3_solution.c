#include "thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "event/thread.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "dht.h"

#define STORAGE_NAMESPACE "storage"

static dht_t dev;

void write_device_state_to_nvs(uint8_t state, nvs_handle_t handle) {
    ESP_ERROR_CHECK(nvs_set_u8(handle, "device_state", state));
    ESP_ERROR_CHECK(nvs_commit(handle));
}

void sensor_loop(void) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        puts(esp_err_to_name(err));
        return;
    }

    int16_t temp, hum;

    while (1) {
        dht_read(&dev, &temp, &hum);

        uint8_t state = (temp % 2 == 0) ? 1 : 0; // Example logic
        write_device_state_to_nvs(state, handle);

        xtimer_sleep(1);
    }

    nvs_close(handle);
}

int main(void) {
    nvs_flash_init();

    // Initialize DHT sensor
    if (dht_init(&dev, &dht_params[0]) != DHT_OK) {
        puts("Failed to init DHT sensor");
        return 1;
    }

    // Start the sensor loop
    sensor_loop();

    return 0;
}