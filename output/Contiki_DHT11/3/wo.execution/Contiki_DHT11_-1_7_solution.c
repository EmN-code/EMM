#define DHT11_PIN 2

#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dht11-sensor.h"
#include "lib/random.h"
#include "sys/timer.h"
#include "sys/etimer.h"
#include "sys/clock.h"
#include "stdio.h" // For logging purposes

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
    static struct etimer timer;
    static int temperature, humidity;

    PROCESS_BEGIN();

    // Initialize the sensor
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, GPIO_HAL_PORT_A);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_PIN);

    while(1) {
        // Set timer for 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read values from the DHT11 sensor
        if (dht11_sensor.status(SENSORS_READY) == DHT11_STATUS_OKAY) {
            humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
            temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
            printf("Humidity: %d.%d %% Temperature: %d.%d C\n",
                    humidity,
                    dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL),
                    temperature,
                    dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL));
        } else {
            printf("Failed to read from DHT11 sensor.\n");
        }
    }

    PROCESS_END();
}