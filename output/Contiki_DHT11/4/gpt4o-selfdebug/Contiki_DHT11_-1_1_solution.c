#include <stdio.h>
#include "contiki.h"
#include "dev/dht11-sensor.h"
#include "dev/gpio.h"

#define DHT11_PIN GPIO_PIN2

PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

PROCESS_THREAD(dht11_reader_process, ev, data)
{
    static struct etimer timer;
    static int temperature, humidity;

    PROCESS_BEGIN();

    // Initialize the GPIO pin for the DHT11 sensor
    gpio_set_input(DHT11_PIN);

    while(1) {
        // Set the timer to 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read temperature and humidity from the DHT11 sensor
        SENSORS_ACTIVATE(dht11_sensor);
        temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
        humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);
        SENSORS_DEACTIVATE(dht11_sensor);

        if(temperature != -1 && humidity != -1) {
            printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }
    }

    PROCESS_END();
}