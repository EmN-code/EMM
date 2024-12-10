
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/dht11-sensor.h"
#include "dev/leds.h"

#define DHT11_GPIO_PORT DHT11_CONFIGURE_GPIO_PORT
#define DHT11_GPIO_PIN DHT11_CONFIGURE_GPIO_PIN

#define RGB_LED_RED   (1 << 0)
#define RGB_LED_GREEN (1 << 1)
#define RGB_LED_BLUE  (1 << 2)
#define RGB_LED_WHITE (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

static void configure_dht11(void) {
    gpio_hal_arch_pin_set_output(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
    gpio_hal_arch_pin_set_input(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
}

static void read_dht11(void) {
    uint8_t humidity_integer, humidity_decimal, temperature_integer, temperature_decimal;
    int status = dht11_sensor.value(&dht11_sensor, DHT11_VALUE_HUMIDITY_INTEGER, &humidity_integer);
    if (status == DHT11_STATUS_OKAY) {
        dht11_sensor.value(&dht11_sensor, DHT11_VALUE_HUMIDITY_DECIMAL, &humidity_decimal);
        dht11_sensor.value(&dht11_sensor, DHT11_VALUE_TEMPERATURE_INTEGER, &temperature_integer);
        dht11_sensor.value(&dht11_sensor, DHT11_VALUE_TEMPERATURE_DECIMAL, &temperature_decimal);
        leds_set(RGB_LED_WHITE);
    } else {
        leds_off(RGB_LED_WHITE);
    }
}

PROCESS_THREAD(dht11_read_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    configure_dht11();

    while (1) {
        etimer_set(&timer, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        read_dht11();
    }

    PROCESS_END();
}

int main(void) {
    process_start(&dht11_read_process, NULL);
    return 0;
}
