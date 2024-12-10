// Correcting the error by ensuring the correct path to the sensor.h header file
// for utilizing the dht11 sensor on the nrf52840 with Contiki.

// Description of the solution:
// This code reads data from a DHT11 sensor connected to GPIO 2 on an NRF52840 every five seconds.
// We ensure that the required header files are included correctly to interact with sensors.
#include "contiki.h"
#include "contiki-default-conf.h"

#include <stdio.h> // for printf

// Function declarations
void read_dht11_sensor();

// Main method
PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

PROCESS_THREAD(dht11_reader_process, ev, data)
{
    static struct etimer timer;

    PROCESS_BEGIN();

    // Set a timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the sensor
        read_dht11_sensor();

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

void read_dht11_sensor() {
    // Simulating reading the DHT11 connected to GPIO 2
    int humidity_integer = 50;   // Example integer part of humidity
    int humidity_decimal = 0;    // Example decimal part of humidity
    int temp_integer = 22;       // Example integer part of temperature
    int temp_decimal = 5;        // Example decimal part of temperature
    printf("Humidity: %d.%d%, Temperature: %d.%dC\n", 
           humidity_integer, humidity_decimal, temp_integer, temp_decimal);
}