#include <stdio.h>
#include <stdlib.h>
#include "thread.h"
#include "dht.h"
#include "nvs_flash.h"
#include "nvs.h"

#define DHT_PIN GPIO_PIN(0, 4)  // Assuming pin 4 is used for DHT sensor
#define NVS_NAMESPACE "storage"

static char _stack[THREAD_STACKSIZE_MAIN];

void write_state_to_nvs(int8_t temp) {
    // Initialize NVS
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    // Update temperature in NVS
    err = nvs_set_i8(nvs_handle, "device_state", temp);
    if (err != ESP_OK) {
        printf("Failed to write to NVS!\n");
    }

    // Commit written value.
    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        printf("Failed to commit to NVS!\n");
    }

    // Close NVS
    nvs_close(nvs_handle);
}

void *dht_thread(void *arg) {
    dht_t dev;
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;
    dht_init(&dev, &params);

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d\n", temp);
            write_state_to_nvs(temp);
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        thread_sleep_s(1); // Sleep for a second
    }
    return NULL;
}

int main(void) {
    printf("Starting DHT read and store to NVS example\n");

    // Initialize the NVS flash
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Start the DHT thread
    thread_create(_stack, sizeof(_stack), THREAD_PRIORITY_MAIN, 0, dht_thread, NULL, "dht_reader");

    return 0;
}