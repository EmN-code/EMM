
#include "contiki.h"
#include "dev/leds.h"
#include "dev/dht11.h"
#include "sys/etimer.h"
#include <stdio.h>

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN  1

#define RGB_LED_RED   0x01
#define RGB_LED_GREEN 0x02
#define RGB_LED_BLUE  0x04

#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)
#define RGB_LED_CYAN    (RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_YELLOW  (RGB_LED_RED | RGB_LED_GREEN)

PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

static struct etimer timer;

void read_dht11_and_set_led() {
    uint8_t data[5];
    if (dht11_read(DHT11_GPIO_PORT, DHT11_GPIO_PIN, data) == DHT11_OK) {
        printf("Temperature: %d.%d C\n", data[DHT11_VALUE_TEMPERATURE_INTEGER], data[DHT11_VALUE_TEMPERATURE_DECIMAL]);
        leds_set(RGB_LED_WHITE); // Turn on the RGB LED
    } else {
        printf("Failed to read DHT11\n");
        leds_set(RGB_LED_RED); // Turn on the RGB LED in red if reading fails
    }
}

PROCESS_THREAD(dht11_rgb_led_process, ev, data) {
    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5); // Set timer to 5 seconds

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        read_dht11_and_set_led();
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main() {
    dht11_init();
    process_start(&dht11_rgb_led_process, NULL);
    return 0;
}
