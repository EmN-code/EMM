#include "dev/rgb_led.h"
#include "dht11_sensor.h"
#include "contiki.h"

#include <stdio.h>

PROCESS(nrf52840_dht11_rgb_led_process, "DHT11 and RGB LED process");
AUTOSTART_PROCESSES(&nrf52840_dht11_rgb_led_process);

PROCESS_THREAD(nrf52840_dht11_rgb_led_process, ev, data)
{
    static struct etimer timer;
    int temperature, humidity;

    PROCESS_BEGIN();

    // Configuring the DHT11 sensor
    SENSORS_ACTIVATE(dht11_sensor);

    while (1) {
        // Set a timer for 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait until the timer expires
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read temperature and humidity values
        temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

        // Check if reading is valid
        if(dht11_sensor.status(0) == DHT11_STATUS_OKAY) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
            // Turn on the RGB LED indicating successful reading
            rgb_led_set(RGB_LED_RED);
        } else {
            printf("Failed to read from DHT11 sensor\n");
            // Turn off the RGB LED indicating a failed reading
            rgb_led_off(RGB_LED_RED);
        }
    }

    PROCESS_END();
}