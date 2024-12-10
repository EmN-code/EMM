#include "contiki.h"
#include "dht11-sensor.h"
#include "dev/gpio-hal.h"
#include "sensors.h"

#define DHT11_PIN 2
#define READ_INTERVAL 5 * CLOCK_SECOND

PROCESS(dht11_read_process, "DHT11 Sensor Reading Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
    static struct etimer timer;
    static int temperature_integer;
    static int temperature_decimal;
    static int humidity_integer;
    static int humidity_decimal;

    PROCESS_BEGIN();

    // Configure DHT11 GPIO Pin
    gpio_hal_arch_no_port_digital_in(DHT11_PIN);

    // Activate the DHT11 sensor
    etimer_set(&timer, READ_INTERVAL);
    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        sensors_event_t dht11_evt;
        dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_PIN);
        SENSORS_ACTIVATE(dht11_sensor);

        // Read sensor
        dht11_evt = sensors_get_data_event(&dht11_sensor);
        temperature_integer = dht11_evt.value[DHT11_VALUE_TEMPERATURE_INTEGER];
        temperature_decimal = dht11_evt.value[DHT11_VALUE_TEMPERATURE_DECIMAL];
        humidity_integer = dht11_evt.value[DHT11_VALUE_HUMIDITY_INTEGER];
        humidity_decimal = dht11_evt.value[DHT11_VALUE_HUMIDITY_DECIMAL];

        // For demonstration purposes; replace with logging functions if needed
        printf("Temperature: %d.%d°C, Humidity: %d.%d%\n", 
                temperature_integer, 
                temperature_decimal, 
                humidity_integer, 
                humidity_decimal);

        SENSORS_DEACTIVATE(dht11_sensor);

        // Reset timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}