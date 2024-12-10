
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/dht11.h"
#include "dev/leds.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN  1

#define RGB_LED_RED     (1 << 0)
#define RGB_LED_GREEN   (1 << 1)
#define RGB_LED_BLUE    (1 << 2)

#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_CYAN    (RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)

PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

void rgb_led_set(uint8_t colour) {
    gpio_hal_arch_port_set_pins(0, colour);
}

PROCESS_THREAD(dht11_rgb_led_process, ev, data) {
    static struct etimer et;
    static uint8_t dht11_data[5];
    static uint8_t colour = 0;

    PROCESS_BEGIN();

    etimer_set(&et, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        if(dht11_read(DHT11_GPIO_PORT, DHT11_GPIO_PIN, dht11_data, sizeof(dht11_data)) == DHT11_OK) {
            colour = RGB_LED_WHITE;
        } else {
            colour = RGB_LED_MAGENTA;
        }

        rgb_led_set(colour);

        etimer_reset(&et);
    }

    PROCESS_END();
}

int main(void) {
    gpio_hal_arch_port_init(0);
    process_start(&dht11_rgb_led_process, NULL);
    return 0;
}
