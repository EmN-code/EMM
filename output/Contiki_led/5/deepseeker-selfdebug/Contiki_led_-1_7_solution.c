
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/leds.h"
#include "dht11-sensor.h"

#include <stdio.h>

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN  1

#define RGB_LED_RED     (1 << 0)
#define RGB_LED_GREEN   (1 << 1)
#define RGB_LED_BLUE    (1 << 2)
#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

static struct etimer et;

void contiki_init(void) {
    // Initialize the DHT11 sensor
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);
}

PROCESS_THREAD(dht11_read_process, ev, data)
{
    PROCESS_BEGIN();

    contiki_init();

    while(1) {
        etimer_set(&et, CLOCK_SECOND * 5); // Set timer for 5 seconds

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Activate the DHT11 sensor
        if(sensors_activate(&dht11_sensor) == 0) {
            // Read the humidity and temperature
            int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
            int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

            printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);

            // Turn on the RGB LED
            leds_set(RGB_LED_WHITE);

            // Deactivate the DHT11 sensor
            sensors_deactivate(&dht11_sensor);
        } else {
            printf("Failed to activate DHT11 sensor\n");
        }
    }

    PROCESS_END();
}

int main(void) {
    // Initialize Contiki
    contiki_init();

    // Start the Contiki system
    contiki_start();

    return 0;
}
