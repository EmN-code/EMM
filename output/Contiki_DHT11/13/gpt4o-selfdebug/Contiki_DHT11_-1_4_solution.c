#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "sys/process.h"
#include "dev/dht11-sensor.h"

#include <stdio.h>

PROCESS(dht11_reading_process, "DHT11 Reading Process");
AUTOSTART_PROCESSES(&dht11_reading_process);

PROCESS_THREAD(dht11_reading_process, ev, data)
{
    static struct etimer timer;
    static int temperature, humidity;

    PROCESS_BEGIN();

    // Initialize the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

        // Read temperature and humidity values
        humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

        // Display the values
        printf("Humidity: %d%%\n", humidity);
        printf("Temperature: %d°C\n", temperature);

        // Reset the timer to trigger again in 5 seconds
        etimer_reset(&timer);
    }

    PROCESS_END();
}