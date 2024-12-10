#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(PORT_A, 5)
#define LED_PIN GPIO_PIN(PORT_B, 6)
#define TEMP_THRESHOLD 30 // Temperature threshold in Celsius

void connect_to_external_service(void) {
    int rc;
    while (1) {
        rc = some_external_connection_function(); // Hypothetical function
        if (rc != 0) {
            printf("Connection attempt failed, retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Connected successfully.\n");
            break;
        }
    }
}

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    int res;

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize DHT11
    res = dht_init(&dev, DHT_PIN, DHT11);
    if (res != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Simulate connecting to an external service using retry logic
    connect_to_external_service();

    while (1) {
        // Read values from DHT11
        res = dht_read(&dev, &temp, &hum);
        if (res == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp/10, temp%10, hum/10, hum%10);
            
            // Check temperature threshold and control LED
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        xtimer_sleep(5); // Wait for 5 seconds before next reading
    }

    return 0;
}