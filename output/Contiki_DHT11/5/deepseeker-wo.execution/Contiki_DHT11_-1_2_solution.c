
#include "sys/etimer.h"
#include "contiki.h"
#include "coap-observe-client.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include "contiki-conf.h"
#include "net/routing/routing.h"
#include "contiki-default-conf.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Define the GPIO pin for DHT11
#define DHT11_GPIO_PIN 2

// Function to read DHT11 sensor data
void read_dht11(uint8_t *humidity, uint8_t *temperature) {
    // Simulated data for demonstration purposes
    *humidity = 50;
    *temperature = 25;
}

// Main function
int main(void) {
    uint8_t humidity, temperature;
    struct etimer timer;

    // Initialize the Contiki system
    contiki_init();

    while (1) {
        // Set the timer to expire after 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor data
        read_dht11(&humidity, &temperature);

        // Print the sensor data
        printf("Humidity: %d %%\n", humidity);
        printf("Temperature: %d C\n", temperature);
    }

    return 0;
}
