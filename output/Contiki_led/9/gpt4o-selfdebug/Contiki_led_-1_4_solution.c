#include net/ipv6/uiplib.h
#include net/ipv6/uip-sr.h
#include net/ipv6/uip.h
#include net/ipv6/uip-udp-packet.h
#include net/ipv6/uip-ds6-nbr.h

#include <stdio.h>
#include "contiki.h"
#include "dev/leds.h"
#include "dev/dht11-sensor.h"

// Function to read data from DHT11 sensor
static int read_dht11(void) {
    int temperature, humidity;
    SENSORS_ACTIVATE(dht11_sensor);
    temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
    humidity = dht11_sensor.value(DHT11_SENSOR_HUM);
    SENSORS_DEACTIVATE(dht11_sensor);
    printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
    return 0; // Indicating success
}

PROCESS(read_dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&read_dht11_process);

PROCESS_THREAD(read_dht11_process, ev, data) {
    static struct etimer timer;
    PROCESS_BEGIN();

    while(1) {
        etimer_set(&timer, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        if (read_dht11() == 0) {
            leds_on(LEDS_RED | LEDS_GREEN | LEDS_BLUE);  // Turn on RGB LED
        }
    }

    PROCESS_END();
}
