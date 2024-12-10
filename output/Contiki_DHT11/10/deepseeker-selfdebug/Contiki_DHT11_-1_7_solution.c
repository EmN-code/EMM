
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/dht11-sensor.h"
#include <stdio.h>

#define DHT11_GPIO_PIN 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
    static struct etimer timer;
    static int humidity_int, humidity_dec, temp_int, temp_dec;

    PROCESS_BEGIN();

    // Configure GPIO pin for DHT11
    gpio_hal_init();
    gpio_hal_set_input(DHT11_GPIO_PIN);

    while (1) {
        etimer_set(&timer, CLOCK_SECOND * 5); // Set timer to 5 seconds

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read DHT11 sensor values
        humidity_int = sensors_sample_fetch_value(&dht11_sensor, DHT11_VALUE_HUMIDITY_INTEGER);
        humidity_dec = sensors_sample_fetch_value(&dht11_sensor, DHT11_VALUE_HUMIDITY_DECIMAL);
        temp_int = sensors_sample_fetch_value(&dht11_sensor, DHT11_VALUE_TEMPERATURE_INTEGER);
        temp_dec = sensors_sample_fetch_value(&dht11_sensor, DHT11_VALUE_TEMPERATURE_DECIMAL);

        printf("Humidity: %d.%d %%, Temperature: %d.%d C\n", humidity_int, humidity_dec, temp_int, temp_dec);
    }

    PROCESS_END();
}

int main(void)
{
    contiki_init();
    contiki_start();
    return 0;
}
