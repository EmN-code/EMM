#include <thread_flags.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <dht.h>
#include <esp_system.h>
#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"

#define DHT_PIN GPIO_PIN(0, 4) // Example GPIO pin
#define STORAGE_NAMESPACE "storage"

static void read_and_store_temperature(nvs_handle_t handle) {
    dht_t dev;
    int16_t temperature, humidity;
    if (dht_init(&dev, &(dht_params_t){ .pin = DHT_PIN, .type = DHT11 }) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return;
    }
    if (dht_read(&dev, &temperature, &humidity) != 0) {
        printf("Failed to read data from DHT sensor\n");
        return;
    }
    printf("Read temperature: %0.1f\n", temperature / 10.0);
    // Convert temperature to a "state" (1 if above 25°C, 0 otherwise)
    uint8_t state = (temperature / 10.0 > 25.0) ? 1 : 0;
    // Store the state in NVS
    esp_err_t err = nvs_set_u8(handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Failed to write device state to NVS\n");
        return;
    }
    err = nvs_commit(handle);
    if (err != ESP_OK) {
        printf("Failed to commit changes in NVS\n");
        return;
    }
    printf("Device state successfully stored: %d\n", state);
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Failed to initialize NVS\n");
        return -1;
    }

    nvs_handle_t handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Failed to open NVS handle\n");
        return -1;
    }

    while (1) {
        read_and_store_temperature(handle);
        xtimer_periodic_wakeup(&last_wakeup, 1000000);
    }

    nvs_close(handle);
    return 0;
}