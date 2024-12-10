#include "sensors.h"
#include "dht11-sensor.h"
#include "contiki.h"
#include "dev/rgb-led.h"
#include <stdio.h>

PROCESS(dht11_read_process, "DHT11 Read and LED Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

        // Try to read the sensor
        if (SENSORS_READY == sensors_activate(&dht11_sensor)) {
            // Read the DHT11 values
            int status = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

            if (status == DHT11_STATUS_OKAY) {
                int temp_int = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
                int temp_dec = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);
                int humid_int = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
                int humid_dec = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);

                printf("Temperature: %d.%d C, Humidity: %d.%d\n", temp_int, temp_dec, humid_int, humid_dec);

                // Turn on RGB LED
                rgb_led_set(RGB_LED_MAGENTA);
            } else {
                printf("Failed to read DHT11 sensor\n");
            }
        }

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}