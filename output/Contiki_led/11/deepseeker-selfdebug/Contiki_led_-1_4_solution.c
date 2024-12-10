
#include "contiki.h"
#include "dev/leds.h"
#include "dht11-sensor.h"
#include <stdio.h>

#define LED_COLOR RGB_LED_WHITE

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
    static struct etimer timer;
    static int humidity, temperature;

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 1);

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

        // Print the readings
        printf("Humidity: %d %%\n", humidity);
        printf("Temperature: %d C\n", temperature);

        // Turn on the RGB LED
        leds_set(LED_COLOR);

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void)
{
    // Initialize Contiki
    contiki_init();

    // Start the Contiki system
    while(1) {
        process_run();
    }

    return 0;
}
