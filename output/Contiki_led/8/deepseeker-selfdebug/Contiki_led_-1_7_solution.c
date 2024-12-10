
#include "contiki.h"
#include "dev/dht11-sensor.h"
#include "dev/leds.h"
#include <stdio.h>

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
    static struct etimer timer;
    static int temp_integer, temp_decimal, hum_integer, hum_decimal;

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 2);

    etimer_set(&timer, CLOCK_SECOND * 5); // Set timer to 5 seconds

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read temperature and humidity from DHT11
        temp_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        temp_decimal = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);
        hum_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        hum_decimal = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);

        // Print the values (optional, to verify data)
        printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", temp_integer, temp_decimal, hum_integer, hum_decimal);

        // Turn on the RGB LED
        leds_on(LEDS_RED);

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void)
{
    // Initialize Contiki
    contiki_init();

    // Start the process
    process_start(&dht11_led_process, NULL);

    // Enter the Contiki main loop
    while (1) {
        process_run();
    }

    return 0;
}
