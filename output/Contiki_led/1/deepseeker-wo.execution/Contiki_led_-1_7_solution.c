
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/gpio-hal.h"
#include "dev/leds.h"
#include "random.h"
#include "net/ipv6/simple-udp.h"
#include "net/netstack.h"
#include "sys/clock.h"
#include "sys/log.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 1
#define RGB_LED_PIN 2

#define READ_INTERVAL (5 * CLOCK_SECOND)

static struct etimer read_timer;

void read_dht11(void) {
    // Simulate reading DHT11 sensor
    // In a real implementation, you would read the sensor data here
    LOG_INFO("DHT11 read simulated\n");
}

void turn_on_rgb_led(void) {
    gpio_hal_set(RGB_LED_PIN);
    LOG_INFO("RGB LED turned on\n");
}

void turn_off_rgb_led(void) {
    gpio_hal_clear(RGB_LED_PIN);
    LOG_INFO("RGB LED turned off\n");
}

PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

PROCESS_THREAD(dht11_reader_process, ev, data) {
    PROCESS_BEGIN();

    // Initialize GPIO for RGB LED
    gpio_hal_init();
    gpio_hal_output(RGB_LED_PIN);

    // Set the initial timer for reading DHT11
    etimer_set(&read_timer, READ_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&read_timer));

        // Read DHT11 sensor
        read_dht11();

        // Turn on RGB LED
        turn_on_rgb_led();

        // Wait for a short period to simulate LED on time
        etimer_set(&read_timer, CLOCK_SECOND / 2);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&read_timer));

        // Turn off RGB LED
        turn_off_rgb_led();

        // Reset the timer for the next reading
        etimer_reset(&read_timer);
    }

    PROCESS_END();
}
