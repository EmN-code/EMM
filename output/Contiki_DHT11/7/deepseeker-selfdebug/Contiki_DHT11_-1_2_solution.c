
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "sys/etimer.h"
#include <stdio.h>

// Assuming dht11.h is available in the project
#include "dev/dht11.h"

// Define the GPIO pin for DHT11
#define DHT11_GPIO_PIN 2

// Function to read DHT11 sensor data
static void read_dht11(void) {
    uint8_t humidity_integer, humidity_decimal, temperature_integer, temperature_decimal;
    int status = dht11_read(DHT11_GPIO_PIN, &humidity_integer, &humidity_decimal, &temperature_integer, &temperature_decimal);

    if (status == DHT11_STATUS_OKAY) {
        printf("DHT11 Sensor Data: Humidity: %d.%d%%, Temperature: %d.%d°C\n",
               humidity_integer, humidity_decimal, temperature_integer, temperature_decimal);
    } else {
        printf("Failed to read DHT11 sensor data. Status: %d\n", status);
    }
}

PROCESS(dht11_process, "DHT11 Sensor Reading Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    // Set up the GPIO pin for DHT11
    gpio_hal_pin_set_output(DHT11_GPIO_PIN);

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor data
        read_dht11();

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    // Initialize the Contiki system
    contiki_init();

    // Start the DHT11 reading process
    process_start(&dht11_process, NULL);

    return 0;
}
