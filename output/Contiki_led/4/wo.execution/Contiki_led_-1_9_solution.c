#include "net/ipv6/uip-ds6-route.h"
#include "mqtt.h"
#include "dev/leds.h"
#include "dev/gpio-hal.h"
#include "coap-engine.h"
#include "contiki.h"
#include "random.h"
#include "sys/etimer.h"
#include "dht11-sensor.h"

// Description:
// This program makes use of the Contiki operating system to run an application on an NRF52840 board that reads a DHT11 sensor to get temperature and humidity values every five seconds, and turns on an RGB LED.

#define SENSOR_READ_INTERVAL (CLOCK_SECOND * 5)

PROCESS(read_dht11_process, "DHT11 Sensor Reading Process");
AUTOSTART_PROCESSES(&read_dht11_process);

PROCESS_THREAD(read_dht11_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    // Configure GPIO port and pin for DHT11
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, GPIO_HAL_PORT_A);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, GPIO_HAL_PIN_0);

    // Setup a periodic timer that expires every 5 seconds
    etimer_set(&timer, SENSOR_READ_INTERVAL);

    while(1) {
        // Wait for the periodic timer
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Activate the DHT11 sensor to read values
        SENSORS_ACTIVATE(dht11_sensor);

        // Get temperature and humidity values from DHT11
        int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

        printf("Temperature: %d*C, Humidity: %d%%\n", temperature, humidity);

        // Turn on RGB LED
        rgb_led_on();

        // Reset the etimer to trigger periodically again
        etimer_reset(&timer);

        // Deactivate the DHT11 sensor
        SENSORS_DEACTIVATE(dht11_sensor);
    }

    PROCESS_END();
}