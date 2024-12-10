#include "contiki.h"
#include "dev/leds.h"
#include "dev/dht11-sensor.h"
#include <stdio.h>

PROCESS(dht11_led_process, "DHT11 and LED Control Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data) {
    static struct etimer timer;
    int temperature, humidity;

    PROCESS_BEGIN();
    
    // Initialize the DHT11 sensor
    SENSORS_ACTIVATE(dht11_sensor);

    while(1) {
        // Set the timer for 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read temperature and humidity from the DHT11 sensor
        temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
        humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

        // Check if sensor readings are valid
        if(temperature != DHT11_ERROR && humidity != DHT11_ERROR) {
            printf("Temperature: %d C, Humidity: %d\n", temperature, humidity);

            // Logic to turn on the RGB LED
            leds_on(LEDS_RED);
            clock_delay_usec(1000 * 1000);  // Delay for 1 second
            leds_off(LEDS_RED);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }
    }

    // Deactivate the DHT11 sensor (not necessary in an infinite loop, but good practice)
    SENSORS_DEACTIVATE(dht11_sensor);

    PROCESS_END();
}