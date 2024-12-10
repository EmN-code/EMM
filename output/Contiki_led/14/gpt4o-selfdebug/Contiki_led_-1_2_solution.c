#include <contiki.h>
#include <contiki-default-conf.h>
#include "dev/leds.h"
#include "dev/dht11.h"

#define RGB_LED_RED     0x01
#define RGB_LED_GREEN   0x02
#define RGB_LED_BLUE    0x04
#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define DHT11_READ_INTERVAL (CLOCK_SECOND * 5)

PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

static void set_rgb_led(uint8_t color) {
    leds_off(RGB_LED_WHITE);
    leds_on(color);
}

PROCESS_THREAD(dht11_rgb_process, ev, data) {
    static struct etimer timer;
    PROCESS_BEGIN();

    dht11_init();

    etimer_set(&timer, DHT11_READ_INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        etimer_reset(&timer);

        int16_t temperature, humidity;
        if (dht11_read(&temperature, &humidity) == DHT11_SUCCESS) {
            if (humidity > 50) {
                set_rgb_led(RGB_LED_GREEN);
            } else {
                set_rgb_led(RGB_LED_RED);
            }
        } else {
            set_rgb_led(RGB_LED_BLUE);
        }
    }

    PROCESS_END();
}