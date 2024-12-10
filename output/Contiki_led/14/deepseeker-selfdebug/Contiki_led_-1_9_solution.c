
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/dht11.h"
#include "dev/leds.h"

#define DHT11_GPIO_PORT (0)
#define DHT11_GPIO_PIN  (1)

#define RGB_LED_RED     (LEDS_RED)
#define RGB_LED_GREEN   (LEDS_GREEN)
#define RGB_LED_BLUE    (LEDS_BLUE)
#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)

PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

PROCESS_THREAD(dht11_rgb_process, ev, data)
{
    static struct etimer timer;
    static uint8_t dht11_data[5];

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    dht11_init(DHT11_GPIO_PORT, DHT11_GPIO_PIN);

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        if(dht11_read(DHT11_GPIO_PORT, DHT11_GPIO_PIN, dht11_data) == 0) {
            // Turn on the RGB LED
            leds_set(RGB_LED_WHITE);
        } else {
            // Turn off the RGB LED
            leds_off(RGB_LED_WHITE);
        }

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void)
{
    // Initialize Contiki OS
    contiki_init();

    // Start the Contiki OS main loop
    while(1) {
        process_run();
    }

    return 0;
}
