
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "sys/etimer.h"
#include <stdio.h>

// Define the GPIO pin for DHT11
#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 2

// Define the status codes for DHT11
#define DHT11_STATUS_OKAY (0)
#define DHT11_STATUS_CHECKSUM_FAILED (2)

// Function to read DHT11 data
int dht11_read(int *temperature, int *humidity) {
    // Simulate reading data from DHT11
    // In a real implementation, this would involve GPIO operations
    *temperature = 25; // Example temperature value
    *humidity = 60;    // Example humidity value

    // Simulate a successful read
    return DHT11_STATUS_OKAY;
}

PROCESS(dht11_process, "DHT11 Reading Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data) {
    static struct etimer timer;
    static int temperature, humidity;
    int status;

    PROCESS_BEGIN();

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        status = dht11_read(&temperature, &humidity);

        if(status == DHT11_STATUS_OKAY) {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
        } else {
            printf("Failed to read DHT11 sensor\n");
        }

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}
