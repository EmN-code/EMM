
#include "contiki.h"
#include "dev/dht11.h"  // Ensure this header file is available
#include "sys/etimer.h"
#include <stdio.h>

#define DHT11_GPIO_PIN 2  // GPIO pin for DHT11

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

static struct etimer timer;

PROCESS_THREAD(dht11_read_process, ev, data)
{
    PROCESS_BEGIN();

    // Configure the DHT11 sensor
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);  // Assuming GPIO port 0
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);

    while (1) {
        etimer_set(&timer, CLOCK_SECOND * 5);  // Set timer for 5 seconds

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        int status = dht11_sensor.status(DHT11_STATUS_OKAY);
        if (status == DHT11_STATUS_OKAY) {
            int humidity_int = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
            int humidity_dec = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);
            int temp_int = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
            int temp_dec = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);

            printf("Humidity: %d.%d %%, Temperature: %d.%d C\n", humidity_int, humidity_dec, temp_int, temp_dec);
        } else {
            printf("Failed to read DHT11 sensor, status: %d\n", status);
        }

        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void)
{
    return 0;
}
