#include "sensors.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-default-conf.h"

static struct etimer timer;

PROCESS(dht11_process, "DHT11 and LED process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data) {
    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        int status = dht11_sensor.status(SENSORS_ACTIVE);
        if (status == DHT11_STATUS_OKAY) {
            int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
            int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        /* Code for turning on RGB LED should be placed here */
        
        etimer_reset(&timer);
    }

    PROCESS_END();
}