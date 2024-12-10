#include "net/netstack.h"
#include "contiki.h"
#include "sys/etimer.h"
#include "nrf_gpio.h"
#include <stdio.h>

// Define the GPIO pin for DHT11 and timer interval
#define DHT11_PIN 2
#define INTERVAL (CLOCK_SECOND * 5)

// Function to initiate reading from DHT11
void read_dht11() {
    // This function should implement the protocol-specific timing to read from DHT11
    // Usually involves sending a start signal and measuring pulsed responses.

    // Placeholder: Simulating read values
    int temperature = 25; // Simulated temperature reading
    int humidity = 60;    // Simulated humidity reading

    // Print the fetched values
    printf("Temperature: %d C, Humidity: %d%%\n", temperature, humidity);
}

PROCESS(dht11_process, "DHT11 Sensor Reading Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
    static struct etimer timer;
    // Initiation steps for the process
    PROCESS_BEGIN();

    // Configure the GPIO for DHT11 sensor
    nrf_gpio_cfg_input(DHT11_PIN, NRF_GPIO_PIN_NOPULL);

    // Set a periodic timer
    etimer_set(&timer, INTERVAL);

    while (1) {
        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read from the DHT11 sensor
        read_dht11();

        // Reset the timer for the next interval
        etimer_reset(&timer);
    }

    PROCESS_END();
}
