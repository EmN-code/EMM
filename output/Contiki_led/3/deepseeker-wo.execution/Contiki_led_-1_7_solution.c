
#include <stdio.h>
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/leds.h"
#include "sys/etimer.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 1
#define RGB_LED_PIN 2

PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

static void read_dht11(void) {
    gpio_hal_set_input(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
    clock_delay_msec(18);
    gpio_hal_set_output(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
    gpio_hal_set(DHT11_GPIO_PORT, DHT11_GPIO_PIN, 0);
    clock_delay_msec(1);
    gpio_hal_set(DHT11_GPIO_PORT, DHT11_GPIO_PIN, 1);
    clock_delay_usec(40);
    gpio_hal_set_input(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
}

static void turn_on_rgb_led(void) {
    gpio_hal_set_output(DHT11_GPIO_PORT, RGB_LED_PIN);
    gpio_hal_set(DHT11_GPIO_PORT, RGB_LED_PIN, 1);
}

PROCESS_THREAD(dht11_rgb_process, ev, data) {
    static struct etimer et;

    PROCESS_BEGIN();

    while(1) {
        etimer_set(&et, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        read_dht11();
        turn_on_rgb_led();
    }

    PROCESS_END();
}

int main(void) {
    gpio_hal_init();
    process_start(&dht11_rgb_process, NULL);
    return 0;
}
