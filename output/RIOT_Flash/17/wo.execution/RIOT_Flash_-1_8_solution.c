#include "xtimer.h"
#include "nvs_flash.h"
#include <esp_wifi.h>
#include <esp_system.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "nvs.h"
#include "dht.h"
#include <inttypes.h>

// DHT sensor configuration
dht_t dev;
dht_params_t dht_params = {
   .pin = GPIO_NUM_4, // Pin number
   .type = DHT22      // DHT22 sensor
};

// Timer setup
xtimer_ticks32_t last_wakeup;

// NVS handle
define NVS_NAMESPACE "storage"
uint8_t device_state = 0;
nvs_handle_t nvs_handle;

// Function to read and store device state
dht_read_task(void *arg) {
    int16_t temp, hum;

    while (true) {
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Error reading values\n");
            continue;
        }

        // Update and store device state
        device_state = (device_state == 0) ? 1 : 0; // Toggle device state
        esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
        if (err != ESP_OK) {
            printf("Error opening NVS handle\n");
            continue;
        }

        err = nvs_set_u8(nvs_handle, "device_state", device_state);
        if (err != ESP_OK) {
            printf("Error setting NVS value\n");
        }

        err = nvs_commit(nvs_handle);
        if (err != ESP_OK) {
            printf("Error committing NVS value\n");
        }

        nvs_close(nvs_handle);

        // Print state
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%, State: %d\n", temp/10, temp%10, hum/10, hum%10, device_state);

        // Wait for next cycle
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ret = nvs_flash_erase();
        if (ret != ESP_OK) {
            printf("Error erasing NVS\n");
            return 1;
        }
        ret = nvs_flash_init();
    }

    if (dht_init(&dev, &dht_params) != DHT_OK) {
        printf("Failed to initialize sensor\n");
        return 1;
    }

    last_wakeup = xtimer_now();

    // Create DHT reading thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, dht_read_task, NULL, "dht_reader");

    return 0;
}