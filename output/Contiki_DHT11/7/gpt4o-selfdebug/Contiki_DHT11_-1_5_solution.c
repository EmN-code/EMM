#include "sensors.h"
#include "dht11-sensor.h"
#include "contiki.h"
#include <stdio.h>

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
    static struct etimer timer;
    int temperature, humidity;

    PROCESS_BEGIN();

    // First, configure the GPIO pin for DHT11
    SENSORS_ACTIVATE(dht11_sensor);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 2);

    // Start the timer with 5 seconds interval
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

        // Read temperature and humidity from DHT11
        temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

        // Print the sensor values
        printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}