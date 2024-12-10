
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/dht11-sensor.h"
#include "dev/leds.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 1

#define RGB_LED_RED   0x01
#define RGB_LED_GREEN 0x02
#define RGB_LED_BLUE  0x04

PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

static void configure_dht11(void) {
    gpio_hal_set(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
    gpio_hal_dir(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_HAL_PIN_OUTPUT);
}

static void read_dht11(void) {
    int status = dht11_sensor.status(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
    if (status == DHT11_STATUS_OKAY) {
        int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);
    } else {
        printf("Failed to read DHT11 sensor\n");
    }
}

static void set_rgb_led(uint8_t color) {
    leds_set(color);
}

PROCESS_THREAD(dht11_rgb_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    configure_dht11();

    while (1) {
        etimer_set(&timer, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        read_dht11();
        set_rgb_led(RGB_LED_RED);
    }

    PROCESS_END();
}

int main(void) {
    contiki_init();
    return 0;
}
