#include xtimer.h
#include time_units.h
#include periph/gpio.h
#include periph/dht.h

#define DHT_PIN GPIO_PIN(0, 4)  // define the pin number connected to DHT11
#define LED_PIN GPIO_PIN(0, 5)  // define the pin number for the LED
#define TEMP_THRESHOLD 30  // temperature threshold to trigger the LED
#define DHT_TYPE DHT11  // specify the sensor type

static dht_t dht_dev;

int main(void) {
    int rc;
    int16_t temp, hum;
    
    // Initialize the DHT sensor
    while (1) {
        rc = dht_init(&dht_dev, DHT_PIN, DHT_TYPE);
        if (rc != DHT_OK) {
            printf("DHT init failed\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("DHT sensor initialized successfully\n");
            break;
        }
    }

    // Configure LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);
    
    while (1) {
        rc = dht_read(&dht_dev, &temp, &hum);
        if (rc != DHT_OK) {
            printf("DHT read failed\n");
        } else {
            // Print the temperature and humidity
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        }

        // Check the temperature
        if (temp > TEMP_THRESHOLD * 10) {
            gpio_set(LED_PIN);
            printf("Temperature exceeds threshold, LED ON\n");
        } else {
            gpio_clear(LED_PIN);
        }

        // Wait for 5 seconds before reading again
        xtimer_sleep(5);
    }
    return 0;
}