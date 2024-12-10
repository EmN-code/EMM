
#include "contiki.h"
#include "dev/dht11.h"  // Ensure this header is available
#include <stdio.h>

// Define the GPIO pin for the DHT11 sensor
#define DHT11_GPIO_PIN 2

// Function to read DHT11 sensor data
void read_dht11(void) {
    int humidity_int, humidity_dec, temperature_int, temperature_dec;
    int status = dht11_read(DHT11_GPIO_PIN, &humidity_int, &humidity_dec, &temperature_int, &temperature_dec);

    if (status == DHT11_STATUS_OKAY) {
        printf("DHT11 Sensor Data: Humidity = %d.%d%%, Temperature = %d.%d°C\n",
               humidity_int, humidity_dec, temperature_int, temperature_dec);
    } else {
        printf("Failed to read DHT11 sensor data. Status: %d\n", status);
    }
}

// Process to handle periodic reading of the DHT11 sensor
PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

PROCESS_THREAD(dht11_reader_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        read_dht11();

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}
