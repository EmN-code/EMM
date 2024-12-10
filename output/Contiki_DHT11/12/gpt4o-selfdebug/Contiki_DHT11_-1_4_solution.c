// A Contiki program to read data from a DHT11 sensor every five seconds on an NRF52840
// The DHT11 is connected to GPIO 2

// Include necessary Contiki header files
#include "sensors.h"
#include "dev/dht11-sensor.h"
#include "contiki.h"

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

static struct etimer timer;

PROCESS_THREAD(dht11_read_process, ev, data)
{
    PROCESS_BEGIN();

    // Initialize the sensor
    SENSORS_ACTIVATE(dht11_sensor);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 2);

    while(1) {
        // Set a timer for 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read values from the sensor
        int humidity_int = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        int humidity_dec = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);
        int temperature_int = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        int temperature_dec = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);

        // Print the results
        printf("Humidity: %d.%d%%\n", humidity_int, humidity_dec);
        printf("Temperature: %d.%d°C\n", temperature_int, temperature_dec);
    }

    PROCESS_END();
}