#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 5) // Assuming DHT11 is connected to PORT_A pin 5
#define LED_PIN GPIO_PIN(PORT_B, 0) // Assuming LED is connected to PORT_B pin 0
#define TEMP_THRESHOLD 30 // Temperature threshold in °C
#define SLEEP_DURATION (5U * US_PER_SEC) // 5 seconds sleep duration

static dht_t dev;

void initialize_peripherals(void) {
    // Initialize the DHT11 sensor
    if (dht_init(&dev, DHT_MODEL_DHT11, DHT_PIN) != DHT_OK) {
        puts("Error initializing DHT11 sensor!");
    }
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN); // Ensure LED is off initially
}

void read_sensor_and_control_led(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);
        if (temp > TEMP_THRESHOLD) {
            gpio_set(LED_PIN); // Turn on LED
        } else {
            gpio_clear(LED_PIN); // Turn off LED
        }
    } else {
        puts("Failed to read from DHT11 sensor!");
    }
}

int main(void) {
    initialize_peripherals();
    
    while (1) {
        read_sensor_and_control_led();
        xtimer_sleep(SLEEP_DURATION);
    }
    
    return 0;
}