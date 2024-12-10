#include <string.h>
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "contiki-net.h"
#include "sys/etimer.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "dht11-sensor.h"

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

static struct etimer timer;

PROCESS_THREAD(dht11_process, ev, data)
{
    PROCESS_BEGIN();

    uint8_t port = GPIO_HAL_PORT_A;
    uint8_t pin = GPIO_HAL_PIN_4;

    etimer_set(&timer, CLOCK_SECOND * 5); // Set a timer for 5 seconds

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Configure the DHT11 sensor
        dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, port);
        dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, pin);

        // Activate the sensor
        SENSORS_ACTIVATE(dht11_sensor);

        // Check the status of the sensor
        if (dht11_sensor.status(DHT11_STATUS_OKAY) == DHT11_STATUS_OKAY) {
            // Read temperature and humidity values
            int temp_int = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
            int temp_dec = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);
            int humidity_int = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
            int humidity_dec = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);

            printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", temp_int, temp_dec, humidity_int, humidity_dec);

            // Turn on the RGB LED
            rgb_led_on();
        }

        // Deactivate the sensor
        SENSORS_DEACTIVATE(dht11_sensor);

        etimer_reset(&timer);
    }

    PROCESS_END();
}