#include <xtimer.h>
#include "periph/gpio.h"
#include "dht.h"
#include <stdio.h>

/* Definitions */
#define DHT_PIN     GPIO_PIN(PORT_B, 0)
#define LED_PIN     GPIO_PIN(PORT_B, 1)
#define TEMP_THRESHOLD 30  // Threshold temperature in Celsius

/* Global Variables */
static dht_t dev;

int main(void)
{
    int16_t temp, hum;
    int rc;
    
    /* Initialize DHT sensor */
    while (1) {
        rc = dht_init(&dev, GPIO_PIN(PORT_B, 0), DHT11);
        if (rc != DHT_OK) {
            printf("DHT initialization failed: %d, retrying...\n", rc);
            xtimer_sleep(1);
            continue;
        } else {
            printf("DHT initialized successfully\n");
            break;
        }
    }
    
    /* Set LED pin to output */
    gpio_init(LED_PIN, GPIO_OUT);
    
    while (1) {
        /* Read temperature and humidity */
        rc = dht_read(&dev, &temp, &hum);
        if (rc == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp/10, temp%10, hum/10, hum%10);
            
            /* Check if temperature exceeds threshold */
            if (temp / 10 > TEMP_THRESHOLD) {
                printf("Temperature exceeds threshold! Turning on LED.\n");
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read data from DHT sensor: %d\n", rc);
        }
        
        /* Wait 5 seconds */
        xtimer_sleep(5);
    }
    
    return 0;
}