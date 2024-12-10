#include "contiki.h"
#include "dev/leds.h"
#include "dev/dht11_sensor.h"
#include <stdio.h>

PROCESS(sensor_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&sensor_led_process);

PROCESS_THREAD(sensor_led_process, ev, data)
{
    static struct etimer timer;
    int temperature, humidity;

    PROCESS_BEGIN();

    while(1) {
        // Set the timer for 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Activate the DHT11 sensor
        SENSORS_ACTIVATE(dht11_sensor);

        // Read temperature and humidity
        temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
        humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

        if(temperature != -1 && humidity != -1) {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

            // Turn on the RGB LED (all colors)
            leds_on(LEDS_ALL);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        // Turn off the RGB LED
        leds_off(LEDS_ALL);

        // Deactivate the DHT11 sensor
        SENSORS_DEACTIVATE(dht11_sensor);
    }

    PROCESS_END();
}

int main() {
    // This example relies on Contiki's process model,
    // which initializes and runs processes automatically.
    return 0;
}