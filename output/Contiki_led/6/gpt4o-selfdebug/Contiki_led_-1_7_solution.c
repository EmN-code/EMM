#include "dev/leds.h"
#include "dht11_sensor.h"
#include "contiki.h"

PROCESS(read_dht11_process, "Read DHT11 Sensor Process");
AUTOSTART_PROCESSES(&read_dht11_process);

PROCESS_THREAD(read_dht11_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        int humidity_integer = DHT11_VALUE_HUMIDITY_INTEGER;
        int humidity_decimal = DHT11_VALUE_HUMIDITY_DECIMAL;
        int temperature_integer = DHT11_VALUE_TEMPERATURE_INTEGER;
        int temperature_decimal = DHT11_VALUE_TEMPERATURE_DECIMAL;

        // Assuming function dht11_read is implemented for interfacing with the sensor
        int status = dht11_read(&humidity_integer, &humidity_decimal, &temperature_integer, &temperature_decimal);

        if(status == DHT11_STATUS_OKAY) {
            // Turn on the RGB LED in white color if the reading is successful
            leds_on(RGB_LED_WHITE);
        } else {
            // Turn off the RGB LED if the reading failed
            leds_off(RGB_LED_WHITE);
        }

        etimer_reset(&timer);
    }

    PROCESS_END();
}