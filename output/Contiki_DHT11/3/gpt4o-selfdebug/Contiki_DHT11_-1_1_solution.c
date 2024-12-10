// Description: This code initializes and reads data from a DHT11 sensor connected to GPIO 2 on an NRF52840 platform every five seconds using Contiki OS.

#include "etimer.h"
#include "sensors.h"
#include "contiki-default-conf.h"

// Reference to the DHT11 sensor
extern const struct sensors_sensor dht11_sensor;

PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

PROCESS_THREAD(dht11_reader_process, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    SENSORS_ACTIVATE(dht11_sensor);

    // Set the timer to expire every five seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor value
        int temperature = dht11_sensor.value(0);
        int humidity = dht11_sensor.value(1);

        // Check if readings are successful
        if (temperature != -1 && humidity != -1) {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}
