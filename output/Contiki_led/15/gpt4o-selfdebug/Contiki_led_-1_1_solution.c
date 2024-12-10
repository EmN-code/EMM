#include "netstack_ip_action.h"
#include "etimer.h"
#include "dht11.h"
#include <stdio.h>

PROCESS(nrf52840_dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&nrf52840_dht11_process);

PROCESS_THREAD(nrf52840_dht11_process, ev, data) {
    static struct etimer timer;
    static int temperature, humidity;

    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        SENSORS_ACTIVATE(dht11_sensor);
        temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
        humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);
        SENSORS_DEACTIVATE(dht11_sensor);

        printf("Temperature: %d C, Humidity: %d %\n", temperature, humidity);

        // Here, you would add code to turn on the RGB LED
        printf("RGB LED is turned on.\n");

        etimer_reset(&timer);
    }

    PROCESS_END();
}