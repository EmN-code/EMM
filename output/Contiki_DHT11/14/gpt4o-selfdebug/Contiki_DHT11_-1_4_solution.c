#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "stdio.h"
#include "dev/dht11-sensor.h"
#include "dev/gpio.h"

PROCESS(dht11_process, "DHT11 process");
AUTOSTART_PROCESSES(&dht11_process);

static struct etimer timer;

PROCESS_THREAD(dht11_process, ev, data) {
    PROCESS_BEGIN();

    SENSORS_ACTIVATE(dht11_sensor);

    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

        if (temperature != -1 && humidity != -1) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        etimer_reset(&timer);
    }

    SENSORS_DEACTIVATE(dht11_sensor);

    PROCESS_END();
}
