
#include "contiki-net.h"
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "contiki-default-conf.h"
#include "tcp-socket.h"
#include "dev/leds.h"
#include "net/linkaddr.h"
#include "dev/rgb-led/rgb-led.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/psock.h"
#include "udp-socket.h"
#include "net/netstack.h"
#include "dev/radio.h"
#include "contiki-conf.h"
#include "dev/button-hal.h"
#include "net/ipv6/tcp-socket.h"
#include "contiki-lib.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 2
#define RGB_LED_PIN 13

#define DHT11_READ_INTERVAL (5 * CLOCK_SECOND)

static struct etimer dht11_timer;

void dht11_init(void) {
    gpio_hal_set_output(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
}

uint8_t dht11_read(void) {
    // Simulated DHT11 read function
    // In a real implementation, this would read the sensor data
    return 0; // Simulated successful read
}

void rgb_led_init(void) {
    gpio_hal_set_output(0, RGB_LED_PIN);
}

void rgb_led_on(void) {
    gpio_hal_set(0, RGB_LED_PIN);
}

void rgb_led_off(void) {
    gpio_hal_clear(0, RGB_LED_PIN);
}

PROCESS(dht11_process, "DHT11 Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data) {
    PROCESS_BEGIN();

    dht11_init();
    rgb_led_init();

    etimer_set(&dht11_timer, DHT11_READ_INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&dht11_timer));

        if (dht11_read() == 0) {
            rgb_led_on();
        } else {
            rgb_led_off();
        }

        etimer_reset(&dht11_timer);
    }

    PROCESS_END();
}

int main(void) {
    contiki_init();
    return 0;
}
