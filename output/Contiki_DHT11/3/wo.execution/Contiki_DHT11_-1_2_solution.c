#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"
#include "coap-request-state.h"
#include "dht11-sensor.h"
#include "sensors.h"
#include "sys/etimer.h"

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
    static struct etimer timer;
    static int temperature, humidity;

    PROCESS_BEGIN();

    // Configure the DHT11 sensor on GPIO port 0, pin 2
    SENSORS_ACTIVATE(dht11_sensor);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 2);

    while(1) {
        // Set timer for 5 seconds
        etimer_set(&timer, 5 * CLOCK_SECOND);
        
        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the sensor data
        temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

        // Print the results
        printf("Temperature: %dC, Humidity: %d%%\n", temperature, humidity);
    }

    SENSORS_DEACTIVATE(dht11_sensor);

    PROCESS_END();
}
