#include <stdio.h>
#include "netstack_ip_action.h"
#include "mqtt_prop_decode_input_props.h"
#include "etimer_adjust.h"
#include "dht11_sensor.h"
#include "contiki-default-conf.h"

PROCESS(dht11_led_process, "DHT11 and LED process");
AUTOSTART_PROCESSES(&dht11_led_process);

static struct etimer et;

PROCESS_THREAD(dht11_led_process, ev, data)
{
    PROCESS_BEGIN();

    while(1) {
        // Set a timer for 5 seconds
        etimer_adjust(&et, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Read sensor data
        int temp = dht11_sensor.value(DHT11_SENSOR_TEMP);
        int hum = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

        if(temp != -1 && hum != -1) {
            printf("Temperature: %d C, Humidity: %d %%\n", temp, hum);
            // Turn on RGB LED here (pseudo-code)
            // rgb_led_on();
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }
    }

    PROCESS_END();
}
