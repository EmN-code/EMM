#include "contiki.h"
#include "dev/leds.h"
#include "sensors/dht11.h"

PROCESS(dht11_example_process, "DHT11 example process");
AUTOSTART_PROCESSES(&dht11_example_process);

PROCESS_THREAD(dht11_example_process, ev, data)
{
    static struct etimer timer;
    static int temperature, humidity;

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    SENSORS_ACTIVATE(dht11_sensor);

    // Set the timer to 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read temperature and humidity from DHT11
        temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

        if (temperature != -1 && humidity != -1) {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

            // Turn on RGB LED (assuming LED_RED, LED_GREEN, LED_BLUE represent the RGB LED)
            leds_on(LEDS_RED);
            leds_on(LEDS_GREEN);
            leds_on(LEDS_BLUE);
        }

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}