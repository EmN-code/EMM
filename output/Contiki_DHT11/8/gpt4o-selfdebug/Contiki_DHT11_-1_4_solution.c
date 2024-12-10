#include "contiki.h"
#include "dht11-sensor.h"
#include "dev/gpio-hal.h"
#include <stdio.h>

// Define constants
#define DHT11_GPIO_PORT GPIO_HAL_PORT_A
#define DHT11_GPIO_PIN  2
#define READ_INTERVAL   (CLOCK_SECOND * 5)

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data) {
    static struct etimer timer;
    static int temperature, humidity;

    PROCESS_BEGIN();

    // Configure the DHT11 sensor
    SENSORS_ACTIVATE(dht11_sensor);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);

    while (1) {
        // Set the timer for the specified interval
        etimer_set(&timer, READ_INTERVAL);

        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Activate the sensor to get new data
        dht11_sensor.configure(SENSORS_ACTIVE, 1);

        // Read temperature and humidity values
        temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

        // Output the values
        printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
    }

    PROCESS_END();
}