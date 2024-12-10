#include <stdio.h>
#include <contiki.h>
#include <dev/dht11-sensor.h>
#include <sys/etimer.h>

PROCESS(dht11_process, "DHT11 Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    // Initialize the sensor
    SENSORS_ACTIVATE(dht11_sensor);

    while (1) {
        // Set the timer for 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the sensor value
        int temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
        int humidity = dht11_sensor.value(DHT11_SENSOR_HUMID);

        // Print the sensor values
        printf("Temperature: %d.%d C, Humidity: %d.%d %%
", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
    }

    SENSORS_DEACTIVATE(dht11_sensor);
    PROCESS_END();
}

int main() {
    process_init();
    process_start(&dht11_process, NULL);
    while(1) {
        process_run();
    }
    return 0;
}