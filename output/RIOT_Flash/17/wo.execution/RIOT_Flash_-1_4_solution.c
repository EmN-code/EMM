#include "ztimer.h"
#include "stddef.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "stdio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "msg.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "unistd.h"
#include "dht_params.h"
#include "dht.h"

#define DHT_TYPE DHT11
#define DHT_PIN  GPIO_PIN(0, 4)
#define SLEEP_INTERVAL SLEEP_SECONDS

void *temp_update_thread(void *arg) {
    (void)arg;
    nvs_handle_t my_handle;
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT_TYPE) != 0) {
        printf("Error initializing DHT sensor\n");
        return NULL;
    }

    // Initialize the NVS flash
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error initializing NVS flash: %s\n", esp_err_to_name(err));
        return NULL;
    }

    // Open NVS storage with a specific name space "storage"
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %s\n", esp_err_to_name(err));
        return NULL;
    }

    while (1) {
        // Read temperature and humidity from the DHT sensor
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Error reading from DHT sensor\n");
        } else {
            // Update the temperature to NVS
            uint8_t state = temp > 25 ? 1 : 0;  // Simple logic example
            err = nvs_set_u8(my_handle, "device_state", state);
            if (err == ESP_OK) {
                nvs_commit(my_handle);
                printf("Stored state %d in NVS\n", state);
            } else {
                printf("Failed to write data to NVS: %s\n", esp_err_to_name(err));
            }
        }
        // Sleep for 1 second
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    // Close NVS handle
    nvs_close(my_handle);
    return NULL;
}

int main(void) {
    // Create a thread for temperature update
    char thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1, 0, temp_update_thread, NULL, "temp_updater");
    return 0;
}