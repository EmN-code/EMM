#include "net/routing/routing.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/simple-udp.h"
#include "contiki.h"
#include "dht11-sensor.h"
#include <stdio.h>

// Configure the RGB LED
#define RGB_LED_PIN 1  // Define the pin connected to your RGB LED

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data){
    static struct etimer timer;

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    SENSORS_ACTIVATE(dht11_sensor);

    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        if(dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, RGB_LED_PIN) &&
           dht11_sensor.status(SENSORS_READY)) {
            int temp = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
            int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
            printf("Temperature: %d C, Humidity: %d %%\n", temp, humidity);

            // Turn on the RGB LED
            leds_on(LEDS_RED);
        }

        etimer_reset(&timer);
    }

    PROCESS_END();
}