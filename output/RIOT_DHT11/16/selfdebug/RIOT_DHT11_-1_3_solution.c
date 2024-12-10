#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h> /* Required for xtimer sleep functionality */
#include <dht.h> /* Include DHT sensor library */

#define DHT_PIN 2 /* Define the GPIO pin connected to the DHT11 */
#define DHT_TYPE DHT11 /* Define the sensor type, ensure proper usage */

/* Define the timeout duration as 5 seconds */
#define TIMEOUT_SEC (5U)

/* Function to read DHT sensor data */
void read_dht_data(void) {
    int16_t temperature = 0;
    int16_t humidity = 0;

    /* Initialize the DHT sensor */
    if (dht_init(DHT_PIN) != DHT_OK) {
        printf("Error initializing DHT sensor\n");
        return;
    }

    /* Read the temperature and humidity data */
    if (dht_read(&temperature, &humidity, DHT_TYPE, DHT_PIN) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temperature / 10, abs(temperature % 10), 
               humidity / 10, abs(humidity % 10));
    } else {
        printf("Error reading DHT sensor\n");
    }
}

int main(void) {
    while(1) {
        read_dht_data();
        xtimer_sleep(TIMEOUT_SEC); /* Sleep for the defined duration */
    }
    return 0;
}