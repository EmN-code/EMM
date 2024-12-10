
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "contiki.h"
#include "dev/dht11-sensor.h"
#include "dev/leds.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 1

#define RGB_LED_RED   0x01
#define RGB_LED_GREEN 0x02
#define RGB_LED_BLUE  0x04

#define RGB_LED_WHITE  (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_YELLOW (RGB_LED_RED | RGB_LED_GREEN)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)
#define RGB_LED_CYAN   (RGB_LED_GREEN | RGB_LED_BLUE)

PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

static void read_dht11(void) {
    uint8_t humidity_integer, humidity_decimal, temperature_integer, temperature_decimal;
    int status = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER, &humidity_integer);
    if (status == DHT11_STATUS_OKAY) {
        dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL, &humidity_decimal);
        dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER, &temperature_integer);
        dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL, &temperature_decimal);
        printf("Humidity: %d.%d %%, Temperature: %d.%d C\n", humidity_integer, humidity_decimal, temperature_integer, temperature_decimal);
    } else {
        printf("Failed to read DHT11 sensor\n");
    }
}

static void turn_on_rgb_led(void) {
    leds_set(RGB_LED_WHITE);
}

PROCESS_THREAD(dht11_rgb_led_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);

    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        read_dht11();
        turn_on_rgb_led();

        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    contiki_init();
    return 0;
}
