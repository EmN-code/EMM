
#include "contiki.h"
#include "contiki-net.h"
#include "dev/gpio-hal.h"
#include "sys/etimer.h"
#include "sys/clock.h"

#define LED_PORT 0
#define LED_PIN 2

#define DHT11_PORT 0
#define DHT11_PIN 3

#define INTERVAL 5 // 5 seconds

static struct etimer et;

void read_dht11(void) {
    gpio_hal_port_t port = DHT11_PORT;
    gpio_hal_pin_t pin = DHT11_PIN;

    // Set the pin to output mode and pull it low for 18ms
    gpio_hal_arch_pin_set_output(port, pin);
    gpio_hal_arch_clear_pin(port, pin);
    clock_delay_usec(18000);

    // Set the pin to input mode and wait for the DHT11 response
    gpio_hal_arch_pin_set_input(port, pin);
    clock_delay_usec(40);

    // Read the data from the DHT11
    uint8_t data[5];
    for (int i = 0; i < 40; i++) {
        // Wait for the start of the bit
        while (gpio_hal_arch_read_pin(port, pin) == 0);
        clock_delay_usec(28);

        // Read the bit
        if (gpio_hal_arch_read_pin(port, pin) == 1) {
            data[i / 8] |= (1 << (7 - (i % 8)));
        }

        // Wait for the end of the bit
        while (gpio_hal_arch_read_pin(port, pin) == 1);
    }

    // Verify checksum
    if ((data[0] + data[1] + data[2] + data[3]) == data[4]) {
        // Valid data, turn on the LED
        gpio_hal_arch_pin_set_output(LED_PORT, LED_PIN);
        gpio_hal_arch_set_pin(LED_PORT, LED_PIN);
    } else {
        // Invalid data, turn off the LED
        gpio_hal_arch_clear_pin(LED_PORT, LED_PIN);
    }
}

PROCESS(dht11_process, "DHT11 Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data) {
    PROCESS_BEGIN();

    // Initialize the LED pin as output and turn it off
    gpio_hal_arch_pin_set_output(LED_PORT, LED_PIN);
    gpio_hal_arch_clear_pin(LED_PORT, LED_PIN);

    // Set the initial timer
    etimer_set(&et, CLOCK_SECOND * INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Read the DHT11 sensor
        read_dht11();

        // Reset the timer for the next reading
        etimer_reset(&et);
    }

    PROCESS_END();
}

int main(void) {
    // Initialize the Contiki system
    process_start(&dht11_process, NULL);
    return 0;
}
