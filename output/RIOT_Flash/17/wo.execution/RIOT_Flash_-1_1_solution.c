#include <stdio.h>  // for printf
#include <stdint.h>  // for uint8_t and int16_t
#include "nvs_flash.h"  // for NVS functions
#include "dht.h"  // for DHT sensor functions
#include "xtimer.h"  // for xtimer_sleep

#define STORAGE_KEY "device_state"
#define NVS_NAMESPACE "storage"
#define DHT_PIN GPIO_PIN(PORT_A, 0) // Example DHT pin init

void initialize_nvs(nvs_handle_t *handle) {
    // Initialize the default NVS partition
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS!\n", esp_err_to_name(err));
    }

    // Open a NVS handle in read and write mode
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
}

void write_device_state(nvs_handle_t handle, uint8_t state) {
    // Write the new state into NVS
    esp_err_t err = nvs_set_u8(handle, STORAGE_KEY, state);
    if (err != ESP_OK) {
        printf("Error (%s) writing to NVS!\n", esp_err_to_name(err));
    }
    
    // Commit written value
    err = nvs_commit(handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing to NVS!\n", esp_err_to_name(err));
    }
}

int main(void) {
    nvs_handle_t my_handle;
    initialize_nvs(&my_handle);

    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT sensor
    dev.pin = DHT_PIN;
    dev.type = DHT11;
    if (dht_init(&dev, NULL) != DHT_OK) {
        printf("DHT initialization failed\n");
        return 1;
    }

    while (1) {
        // Read from the sensor
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read data from DHT sensor\n");
        }
        else {
            // Calculate a simple device state based on temperature
            uint8_t state = (temp > 250) ? 1 : 0; // Just a dummy condition
            write_device_state(my_handle, state);

            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp/10, temp%10, hum/10, hum%10);
        }

        // Sleep for 1 second
        xtimer_sleep(1);
    }

    nvs_close(my_handle); // Close NVS handle
    return 0;
}
