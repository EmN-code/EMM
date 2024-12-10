#include <stdio.h>
#include <string.h>
#include "netstack.h"
#include "contiki.h"
#include "dev/gpio.h"
#include "dev/sensors/dht11.h"

#define DHT11_PIN 2

PROCESS(read_dht11_process, "DHT11 Reader");
AUTOSTART_PROCESSES(&read_dht11_process);

PROCESS_THREAD(read_dht11_process, ev, data)
{
    static struct etimer timer;
    static int temperature, humidity;

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor driver
    SENSORS_ACTIVATE(dht11_sensor);

    while (1) {
        // Set the timer for 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait until the timer expires
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read temperature and humidity from DHT11
        if (dht11_read(&temperature, &humidity, DHT11_PIN) == 0) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }
    }

    SENSORS_DEACTIVATE(dht11_sensor);
    PROCESS_END();
}

int main(void) {
    process_start(&read_dht11_process, NULL);
    return 0;
}