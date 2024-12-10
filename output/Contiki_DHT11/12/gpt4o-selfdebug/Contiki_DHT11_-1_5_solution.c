#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dht11-sensor.h"
#include <stdio.h>

#define GPIO_PORT    0  // Assuming GPIO Port 0 for NRF52840
#define GPIO_PIN     2
#define READ_INTERVAL (CLOCK_SECOND * 5)

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    SENSORS_ACTIVATE(dht11_sensor);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, GPIO_PORT);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, GPIO_PIN);

    etimer_set(&timer, READ_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        if(dht11_sensor.status(SENSORS_READY)) {
            int temp_int = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
            int temp_dec = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);
            int hum_int = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
            int hum_dec = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);

            printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", temp_int, temp_dec, hum_int, hum_dec);
        } else {
            printf("DHT11 sensor not ready\n");
        }

        etimer_reset(&timer);
    }

    PROCESS_END();
}