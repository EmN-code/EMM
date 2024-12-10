#include "dev/leds.h"
#include "contiki.h"
#include "contiki-default-conf.h"
#include <stdio.h>
#include "dht11-sensor.h"

PROCESS(read_dht11_process, "DHT11 Sensor Reading Process");
AUTOSTART_PROCESSES(&read_dht11_process);

PROCESS_THREAD(read_dht11_process, ev, data) {
    static struct etimer timer;
    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Starting DHT11 sensor
        SENSORS_ACTIVATE(dht11_sensor);
        
        // Read temperature and humidity
        int temp_int = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        int hum_int = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

        if (temp_int != -1 && hum_int != -1) {
            printf("Temperature: %d C, Humidity: %d %%\n", temp_int, hum_int);

            // Turn on RGB LED to white
            leds_on(RGB_LED_WHITE);
        } else {
            printf("Failed to read data from DHT11 sensor\n");
        }

        // Stop DHT11 sensor before sleeping
        SENSORS_DEACTIVATE(dht11_sensor);

        etimer_reset(&timer);
    }

    PROCESS_END();
}