
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/gpio-hal.h"
#include "dht11-sensor.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 2

static struct etimer et;

void read_dht11(void) {
    static int last_humidity_integer = -1;
    static int last_temperature_integer = -1;

    int humidity_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    int temperature_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

    if (humidity_integer != last_humidity_integer || temperature_integer != last_temperature_integer) {
        last_humidity_integer = humidity_integer;
        last_temperature_integer = temperature_integer;

        // Log the values
        char log_msg[64];
        snprintf(log_msg, sizeof(log_msg), "Humidity: %d, Temperature: %d", humidity_integer, temperature_integer);
        coap_log_string(log_msg, strlen(log_msg));
    }
}

PROCESS(dht11_process, "DHT11 Reading Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data) {
    PROCESS_BEGIN();

    // Configure DHT11 GPIO
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);

    // Set the sensor active
    dht11_sensor.configure(SENSORS_ACTIVE, 1);

    // Set the timer for 5 seconds
    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Read the DHT11 sensor
        read_dht11();

        // Reset the timer
        etimer_reset(&et);
    }

    PROCESS_END();
}
