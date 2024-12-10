
#include <stdio.h>
#include <stdint.h>
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/leds.h"
#include "sys/etimer.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 1
#define RGB_LED_RED 0
#define RGB_LED_GREEN 1
#define RGB_LED_BLUE 2

#define DHT11_VALUE_CHECKSUM 4
#define RGB_LED_COLOR_RED RGB_LED_RED

static uint8_t dht11_read_byte(void) {
    uint8_t byte = 0;
    for(int i = 0; i < 8; i++) {
        while(!gpio_hal_read(DHT11_GPIO_PORT, DHT11_GPIO_PIN));
        clock_delay_usec(40);
        if(gpio_hal_read(DHT11_GPIO_PORT, DHT11_GPIO_PIN)) {
            byte |= (1 << (7 - i));
        }
        while(gpio_hal_read(DHT11_GPIO_PORT, DHT11_GPIO_PIN));
    }
    return byte;
}

static int dht11_read(uint8_t *humidity, uint8_t *temperature) {
    uint8_t data[5];
    gpio_hal_set_output(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
    gpio_hal_set(DHT11_GPIO_PORT, DHT11_GPIO_PIN, 0);
    clock_delay_msec(20);
    gpio_hal_set(DHT11_GPIO_PORT, DHT11_GPIO_PIN, 1);
    clock_delay_usec(40);
    gpio_hal_set_input(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
    if(!gpio_hal_read(DHT11_GPIO_PORT, DHT11_GPIO_PIN)) {
        return -1;
    }
    while(gpio_hal_read(DHT11_GPIO_PORT, DHT11_GPIO_PIN));
    while(!gpio_hal_read(DHT11_GPIO_PORT, DHT11_GPIO_PIN));
    while(gpio_hal_read(DHT11_GPIO_PORT, DHT11_GPIO_PIN));
    for(int i = 0; i < 5; i++) {
        data[i] = dht11_read_byte();
    }
    gpio_hal_set_output(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
    gpio_hal_set(DHT11_GPIO_PORT, DHT11_GPIO_PIN, 1);
    if(data[4] == (data[0] + data[1] + data[2] + data[3])) {
        *humidity = data[0];
        *temperature = data[2];
        return 0;
    }
    return -1;
}

PROCESS(dht11_rgb_led_process, "DHT11 RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

PROCESS_THREAD(dht11_rgb_led_process, ev, data) {
    static struct etimer et;
    uint8_t humidity, temperature;

    PROCESS_BEGIN();

    etimer_set(&et, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        if(dht11_read(&humidity, &temperature) == 0) {
            printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);
            leds_on(LEDS_RED);
        } else {
            printf("Failed to read DHT11 sensor\n");
        }

        etimer_reset(&et);
    }

    PROCESS_END();
}

int main(void) {
    gpio_hal_init();
    leds_init();
    process_start(&dht11_rgb_led_process, NULL);
    return 0;
}
