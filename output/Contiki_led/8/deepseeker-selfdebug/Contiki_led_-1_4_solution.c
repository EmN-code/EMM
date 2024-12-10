
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/leds.h"
#include "sys/etimer.h"
#include "sys/process.h"
#include "sys/clock.h"

// Include the DHT11 sensor header if available
// #include "dev/dht11.h"

// Define the GPIO pins for the RGB LED
#define RGB_LED_RED_PIN    GPIO_PIN(0, 13)
#define RGB_LED_GREEN_PIN  GPIO_PIN(0, 14)
#define RGB_LED_BLUE_PIN   GPIO_PIN(0, 15)

// Define the GPIO pin for the DHT11 sensor
#define DHT11_PIN          GPIO_PIN(0, 16)

// Define the RGB LED colors
#define RGB_LED_RED        (1 << 0)
#define RGB_LED_GREEN      (1 << 1)
#define RGB_LED_BLUE       (1 << 2)
#define RGB_LED_WHITE      (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_CYAN       (RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_MAGENTA    (RGB_LED_RED | RGB_LED_BLUE)

// Define the DHT11 status codes
#define DHT11_STATUS_OKAY               (0)
#define DHT11_STATUS_TIMEOUT            (1)
#define DHT11_STATUS_CHECKSUM_FAILED    (2)

// Function to initialize the GPIO pins
void gpio_init(void) {
    gpio_hal_set_input(DHT11_PIN);
    gpio_hal_set_output(RGB_LED_RED_PIN);
    gpio_hal_set_output(RGB_LED_GREEN_PIN);
    gpio_hal_set_output(RGB_LED_BLUE_PIN);
}

// Function to read the DHT11 sensor
int dht11_read(void) {
    // Placeholder function for DHT11 read
    // This function should be implemented according to the DHT11 sensor's protocol
    // For now, it returns a success status
    return DHT11_STATUS_OKAY;
}

// Function to control the RGB LED
void rgb_led_control(uint8_t color) {
    gpio_hal_write(RGB_LED_RED_PIN, (color & RGB_LED_RED) ? 1 : 0);
    gpio_hal_write(RGB_LED_GREEN_PIN, (color & RGB_LED_GREEN) ? 1 : 0);
    gpio_hal_write(RGB_LED_BLUE_PIN, (color & RGB_LED_BLUE) ? 1 : 0);
}

// Process to handle the periodic reading of the DHT11 sensor and controlling the RGB LED
PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

PROCESS_THREAD(dht11_rgb_process, ev, data) {
    static struct etimer timer;
    static int dht11_status;

    PROCESS_BEGIN();

    gpio_init();

    etimer_set(&timer, CLOCK_SECOND * 5); // Set timer for 5 seconds

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        dht11_status = dht11_read();

        if (dht11_status == DHT11_STATUS_OKAY) {
            rgb_led_control(RGB_LED_GREEN); // Turn on green LED if reading is successful
        } else {
            rgb_led_control(RGB_LED_RED); // Turn on red LED if reading fails
        }

        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    process_init();
    process_start(&etimer_process, NULL);
    process_start(&dht11_rgb_process, NULL);
    while(1) {
        process_run();
    }
    return 0;
}
