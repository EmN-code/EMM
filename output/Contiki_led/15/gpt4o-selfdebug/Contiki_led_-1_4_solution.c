/* Description: This Contiki application reads the DHT11 sensor every 5 seconds and
 * controls an RGB LED accordingly using NRF52840.
 */

#include "sensors.h"
#include "net/netstack.h"
#include "dev/leds.h"

#include <stdio.h>

/* event timer and process declaration */
static struct etimer timer;
PROCESS(dht11_led_control_process, "DHT11 LED Control Process");
AUTOSTART_PROCESSES(&dht11_led_control_process);

PROCESS_THREAD(dht11_led_control_process, ev, data)
{
    PROCESS_BEGIN();
    /* set the timer for 5 seconds */
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        /* wait for the timer to expire */
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        /* reset the timer */
        etimer_reset(&timer);

        /* check sensor status */
        int status = dht11_sensor.status(SENSOR_READ);

        if (status == DHT11_STATUS_OKAY) {
            int temp_int = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
            int temp_dec = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);
            int hum_int = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
            int hum_dec = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);

            printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", temp_int, temp_dec, hum_int, hum_dec);

            /* Turn on RGB LED for indication */
            leds_on(RGB_LED_MAGENTA);
        } else {
            printf("Failed to read from DHT11 sensor: %d\n", status);
        }
    }

    PROCESS_END();
}