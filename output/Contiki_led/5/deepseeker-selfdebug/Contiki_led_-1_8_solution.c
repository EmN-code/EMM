
#include "contiki.h"
#include "dev/gpio.h"
#include "dev/dht11-sensor.h"
#include "sys/etimer.h"
#include "sys/clock.h"

#define LED_PIN GPIO_PIN(0, 13) // Example GPIO pin for RGB LED

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

static struct etimer et;

void turn_on_led(uint8_t color) {
    gpio_set(LED_PIN);
}

void turn_off_led() {
    gpio_clear(LED_PIN);
}

PROCESS_THREAD(dht11_led_process, ev, data) {
    static uint8_t humidity, temperature;

    PROCESS_BEGIN();

    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up the periodic timer for 5 seconds
    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Read DHT11 sensor
        if (dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER, &humidity) == DHT11_STATUS_OKAY &&
            dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER, &temperature) == DHT11_STATUS_OKAY) {
            // Turn on the LED
            turn_on_led(RGB_LED_WHITE);

            // Print the values
            printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);

            // Turn off the LED after a short period
            etimer_set(&et, CLOCK_SECOND);
            PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
            turn_off_led();
        } else {
            printf("Failed to read DHT11 sensor\n");
        }

        // Reset the timer for the next 5 seconds
        etimer_reset(&et);
    }

    PROCESS_END();
}

int main() {
    // Initialize the Contiki system
    process_init();
    process_start(&etimer_process, NULL);
    process_start(&dht11_led_process, NULL);

    // Main loop
    while (1) {
        process_run();
    }

    return 0;
}
