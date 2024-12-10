#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "sys/timer.h"
#include "contiki-lib.h"
#include "contiki-conf.h"
#include "contiki-default-conf.h"

#include <stdio.h>
#include "nrf.h"
#include "dev/leds.h"
#include "dev/dht11.h"

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
    static struct etimer timer;
    int16_t temperature, humidity;
  
    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

        if(dht11_read(&temperature, &humidity) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

            // Example: Turn on yellow LED if temperature is above 25
            if(temperature > 25) {
                leds_on(RGB_LED_YELLOW);
            } else {
                leds_off(RGB_LED_YELLOW);
            }
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        etimer_reset(&timer);
    }
    
    PROCESS_END();
}