#include "contiki.h"
#include "dev/leds.h"
#include "dev/leds_rgb.h"
#include "dev/dht11-sensor.h"

#include <stdio.h>

PROCESS(dht11_led_process, "DHT11 and LED control");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
    static struct etimer timer;
    static int temperature, humidity;

    PROCESS_BEGIN();
    
    SENSORS_ACTIVATE(dht11_sensor);
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
        humidity = dht11_sensor.value(DHT11_SENSOR_HUM);

        if(temperature != -1 && humidity != -1) {
            printf("Temperature: %d C, Humidity: %d \n", temperature, humidity);
            leds_rgb_on(LEDS_RGB_BLUE);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        etimer_reset(&timer);
    }

    SENSORS_DEACTIVATE(dht11_sensor);
    PROCESS_END();
}