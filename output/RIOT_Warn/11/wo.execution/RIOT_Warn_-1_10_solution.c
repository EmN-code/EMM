#include <esp_system.h>
#include <periph/gpio.h>
#include <stdio.h>

#define DHT_PIN         GPIO_NUM_4   // Change as per your setup
#define LED_PIN         GPIO_NUM_2   // Change as per your setup
#define TEMP_THRESHOLD  30           // Change temperature threshold as needed

void read_and_control(void) {
    int16_t temperature, humidity;
    dht_t dev;
    esp_err_t res;

    // Initialize DHT device descriptor
    dev.pin = DHT_PIN;
    dev.type = DHT11;

    // Assuming there's an appropriate function to initialize the GPIO
    // to its default state prior to reading.

    while (1) {
        res = dht_read(&dev, &temperature, &humidity);
        if (res != ESP_OK) {
            printf("Could not read data from sensor\n");
        }
        else {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        // Simulate a delay, in place of ztimer functionality
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void app_main() {
    // Setup LED GPIO
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Create a thread for repeated reading and controlling
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0, read_and_control, NULL, "read_ctrl_thread");
}