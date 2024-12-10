
#include "contiki.h"
#include "contiki-net.h"
#include "dev/gpio-hal.h"
#include "dev/leds.h"
#include "dev/rgb-led/rgb-led.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 2

#define LED_RED_PIN 1
#define LED_GREEN_PIN 2
#define LED_BLUE_PIN 3

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

static void dht11_init(void) {
    gpio_hal_set_input(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
}

static int dht11_read(void) {
    // Simulated DHT11 read function
    // Replace this with actual DHT11 reading logic
    return 25; // Simulated temperature value
}

static void rgb_led_on(void) {
    gpio_hal_set_output(DHT11_GPIO_PORT, LED_RED_PIN, 1);
    gpio_hal_set_output(DHT11_GPIO_PORT, LED_GREEN_PIN, 1);
    gpio_hal_set_output(DHT11_GPIO_PORT, LED_BLUE_PIN, 1);
}

static void rgb_led_off(void) {
    gpio_hal_set_output(DHT11_GPIO_PORT, LED_RED_PIN, 0);
    gpio_hal_set_output(DHT11_GPIO_PORT, LED_GREEN_PIN, 0);
    gpio_hal_set_output(DHT11_GPIO_PORT, LED_BLUE_PIN, 0);
}

PROCESS_THREAD(dht11_read_process, ev, data) {
    static struct etimer timer;
    static int temperature;

    PROCESS_BEGIN();

    dht11_init();
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        temperature = dht11_read();
        printf("Temperature: %d C\n", temperature);

        rgb_led_on();
        etimer_reset(&timer);

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        rgb_led_off();
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    process_start(&dht11_read_process, NULL);
    return 0;
}
