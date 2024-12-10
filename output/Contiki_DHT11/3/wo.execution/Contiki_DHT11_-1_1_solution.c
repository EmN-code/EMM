#include <stdio.h>
#include "sensors.h"
#include "contiki.h"

#define DHT11_SENSOR_PIN 2
#define READ_INTERVAL (CLOCK_SECOND * 5)

PROCESS(dht11_process, "DHT11 Sensor Reading Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    while(1) {
        etimer_set(&timer, READ_INTERVAL);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        int temperature, humidity;
        if (dht11_read(&temperature, &humidity, DHT11_SENSOR_PIN) == 0) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }
    }

    PROCESS_END();
}

int dht11_read(int *temperature, int *humidity, int pin)
{
    // Simulated read function for the DHT11 sensor on a specific pin.
    // Returns 0 on success and -1 on failure. For real hardware, this should
    // be replaced with actual sensor interface code.
    if (pin == DHT11_SENSOR_PIN) {
        *temperature = 25; // Simulated temperature
        *humidity = 50;    // Simulated humidity
        return 0;
    } else {
        return -1;
    }
}

int main(void) {
    // This function usually initializes the platform and starts the Contiki
    // event scheduler. In this simple example, it's used to explicitly start
    // the process for testing purposes.
    process_start(&dht11_process, NULL);
    while(1) { 
        process_run();
        etimer_request_poll();
    }
    return 0;
}
