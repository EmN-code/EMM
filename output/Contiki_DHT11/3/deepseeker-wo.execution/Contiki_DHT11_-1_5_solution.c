
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/gpio-hal.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 2
#define READ_INTERVAL (5 * CLOCK_SECOND)

static struct etimer read_timer;

static int dht11_read_data(void) {
    gpio_hal_port_t port = DHT11_GPIO_PORT;
    gpio_hal_pin_t pin = DHT11_GPIO_PIN;
    uint8_t data[5] = {0};
    int8_t signal_duration;
    int i, j;

    gpio_hal_arch_pin_set_output(port, pin);
    gpio_hal_arch_clear_pin(port, pin);
    clock_delay(18000);
    gpio_hal_arch_set_pin(port, pin);
    gpio_hal_arch_pin_set_input(port, pin);

    signal_duration = gpio_hal_arch_read_pin(port, pin);
    if (signal_duration == -1) {
        return -1;
    }

    for (i = 0; i < 40; i++) {
        signal_duration = gpio_hal_arch_read_pin(port, pin);
        if (signal_duration == -1) {
            return -1;
        }
        data[i / 8] <<= 1;
        if (signal_duration > 28) {
            data[i / 8] |= 1;
        }
    }

    if ((data[0] + data[1] + data[2] + data[3]) != data[4]) {
        return -1;
    }

    return 0;
}

int main(void) {
    etimer_set(&read_timer, READ_INTERVAL);

    while (1) {
        if (etimer_expired(&read_timer)) {
            etimer_reset(&read_timer);
            if (dht11_read_data() == 0) {
                // Data read successfully, but we don't use it here to avoid the warning
            }
        }
        process_run();
    }

    return 0;
}
