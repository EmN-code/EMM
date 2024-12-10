#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "nvs.h"
#include "dht.h"

#define STORAGE_NAMESPACE "storage"
#define KEY "device_state"
#define SENS_PIN GPIO_PIN(PORT_A, 0) // Assume the DHT sensor is on this pin

static nvs_handle_t my_handle;
static dht_t dev;

dht_params_t param = {
    .pin = SENS_PIN,
    .type = DHT11
};

void setup_nvs(void){
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle!");
    }
}

void store_temperature(void){
    int16_t temp = 0;
    int16_t hum = 0;
    int read_err = dht_read(&dev, &temp, &hum);
    if (read_err == DHT_OK) {
        printf("Temperature: %d, Humidity: %d\n", temp, hum);
        esp_err_t err = nvs_set_i32(my_handle, KEY, temp);
        if (err == ESP_OK) {
            nvs_commit(my_handle);
            printf("Stored temperature: %d\n", temp);
        } else {
            printf("Failed to write temperature to NVS!\n");
        }
    } else {
        printf("Failed to read from DHT sensor!\n");
    }
}

int main(void) {
    dht_init(&dev, &param);
    setup_nvs();

    while(1){
        store_temperature();
        xtimer_sleep(1); // Sleep for 1 second
    }

    nvs_close(my_handle);
    return 0;
}