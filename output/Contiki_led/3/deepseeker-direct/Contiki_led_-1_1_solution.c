
#include "contiki.h"
#include "dev/leds.h"
#include "dev/dht11.h"
#include "dev/nrf52840/nrf52840-gpio.h"
#include <stdio.h>

#define DHT11_PIN 2  // Assuming DHT11 is connected to GPIO pin 2
#define RGB_LED_PIN 3  // Assuming RGB LED is connected to GPIO pin 3

PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

static void init_gpio(void) {
    nrf52840_gpio_configure(DHT11_PIN, NRF_GPIO_PIN_DIR_INPUT, NRF_GPIO_PIN_INPUT_CONNECT, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);
    nrf52840_gpio_configure(RGB_LED_PIN, NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);
}

PROCESS_THREAD(dht11_rgb_process, ev, data)
{
    static struct etimer timer;
    static int temperature, humidity;

    PROCESS_BEGIN();

    init_gpio();

    while(1) {
        etimer_set(&timer, CLOCK_SECOND * 5);  // Set timer for 5 seconds

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        if(dht11_read(&temperature, &humidity, DHT11_PIN) == DHT11_SUCCESS) {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
            nrf52840_gpio_set(RGB_LED_PIN);  // Turn on RGB LED
        } else {
            printf("Failed to read DHT11 sensor\n");
        }

        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void)
{
    process_init();
    process_start(&etimer_process, NULL);
    process_start(&dht11_rgb_process, NULL);
    autostart_start(autostart_processes);

    while(1) {
        process_run();
    }

    return 0;
}
