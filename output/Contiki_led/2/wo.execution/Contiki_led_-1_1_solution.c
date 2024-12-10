#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/simple-udp.h"
#include "contiki-net.h"
#include "contiki-default-conf.h"

#include <stdio.h>
#include "dev/leds.h"
#include "sys/etimer.h"
#include "sensors.h"
#include "dht11.h"

#define READ_INTERVAL (CLOCK_SECOND * 5)

PROCESS(nrf52840_dht11_led_process, "DHT11 LED Control Process");
AUTOSTART_PROCESSES(&nrf52840_dht11_led_process);

PROCESS_THREAD(nrf52840_dht11_led_process, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    SENSORS_ACTIVATE(dht11_sensor);

    while(1) {
        // Set the timer for tracking when to read the sensor next
        etimer_set(&timer, READ_INTERVAL);

        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read values from the DHT11 sensor
        int16_t temperature, humidity;
        if(dht11_read(&temperature, &humidity) == 0) {
            printf("Temperature: %d C, Humidity: %d\%\n", temperature, humidity);

            // Turn on the RGB LED as an indicator
            leds_on(LEDS_RED | LEDS_GREEN | LEDS_BLUE);
        }

        // Wait a bit before turning off the LED so it is visible
        etimer_set(&timer, CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        leds_off(LEDS_RED | LEDS_GREEN | LEDS_BLUE);
    }

    SENSORS_DEACTIVATE(dht11_sensor);

    PROCESS_END();
}
