#include <stdio.h>
#include <string.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "dht.h"
#include "xtimer.h"

#define STORAGE_NAMESPACE "storage"
#define DHT_PIN GPIO_PIN(PORT_A, 9)
#define DHT_TYPE DHT11
#define DELAY 1000000

void update_device_state(nvs_handle_t my_handle, int16_t temp) {
    esp_err_t err;
    err = nvs_set_i16(my_handle, "temperature", temp);
    if (err != ESP_OK) {
        printf("Error (%s) writing temperature to NVS!\n", esp_err_to_name(err));
    }

    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing updates in NVS!\n", esp_err_to_name(err));
    }
    else {
        printf("Temperature %d committed successfully.\n", temp);
    }
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS!\n", esp_err_to_name(err));
        return err;
    }

    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    dht_t dev;
    dht_init(&dev, DHT_PIN, DHT_TYPE);

    while (1) {
        int16_t temp;
        if (dht_read(&dev, &temp, NULL) == DHT_OK) {
            printf("Temperature: %d\n", temp);
            update_device_state(my_handle, temp);
        } else {
            printf("Failed to read from DHT sensor!\n");
        }

        xtimer_usleep(DELAY);
    }

    nvs_close(my_handle);
    return 0;
}