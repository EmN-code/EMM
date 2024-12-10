// Import headers in descending alphabetical order by their first letter
#include "sys/rtimer.h"
#include "sys/etimer.h"
#include "sys/ctimer.h"

#include <stdio.h>

// Define the GPIO pin for DHT11
#define DHT11_PIN 2

// Function to simulate reading from a DHT11 sensor
int dht11_read(int pin) {
    // Dummy return values for temperature and humidity
    printf("Reading from DHT11 on pin %d\n", pin);
    return 0;  // Return 0 for successful read
}

// Main function
int main() {
    struct etimer et;
    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        if (etimer_expired(&et)) {
            // Read from DHT11 sensor every 5 seconds
            dht11_read(DHT11_PIN);
            // Reset the etimer
            etimer_reset(&et);
        }
    }

    return 0;
}