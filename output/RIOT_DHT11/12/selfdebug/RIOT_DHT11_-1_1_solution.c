#include <timex.h>
#include <stdio.h>
#include <dht.h>

// Pin for the DHT11 sensor
#define DHT11_PIN GPIO_PIN(0, 2)

// DHT device descriptor
static dht_t dev;

// Timer interval
#define TIMER_INTERVAL (5000U * US_PER_MS)

// Timer structure
static xtimer_t timer;

// Function to read DHT11 data
static void dht11_read(void *arg) {
    (void)arg;
    int16_t temp, hum;
    int result = dht_read(&dev, &temp, &hum);
    if (result == DHT_OK) {
        printf("Temperature: %.1f°C, Humidity: %.1f%%\n", temp / 10.0, hum / 10.0);
    } else {
        printf("Failed to read from DHT sensor\n");
    }
    xtimer_set_msg(&timer, TIMER_INTERVAL, 0, dht11_read, 0);
}

int main(void) {
    // Initialize DHT11 sensor
    dht_params_t params;
    params.pin = DHT11_PIN;
    params.type = DHT11;
    if (dht_init(&dev, &params) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Start periodic timer
    xtimer_set_msg(&timer, TIMER_INTERVAL, 0, dht11_read, 0);

    printf("DHT11 sensor initialized on GPIO %d\n", DHT11_PIN);

    // Run indefinitely
    while (1) {
        // The timer will handle periodic reads
        xtimer_sleep(60);
    }

    return 0;
}
