
#include "xtimer.h"  // Include the xtimer header to resolve the error
#include "dht11.h"   // Include the DHT11 sensor header
#include "periph/gpio.h"  // Include GPIO peripheral header for RGB LED control

#define DHT11_GPIO_PIN GPIO_PIN(DHT11_CONFIGURE_GPIO_PORT, DHT11_CONFIGURE_GPIO_PIN)
#define RGB_LED_PIN GPIO_PIN(0, 1)  // Example GPIO pin for RGB LED

void read_dht11(void) {
    int16_t temperature, humidity;
    dht11_read(&temperature, &humidity, DHT11_GPIO_PIN);
    printf("Temperature: %d.%d C, Humidity: %d.%d %%\n",
           temperature / 10, temperature % 10, humidity / 10, humidity % 10);
}

void rgb_led_control(void) {
    static uint8_t colour = 0;
    colour = (colour + 1) % 8;  // Cycle through 8 possible colours
    rgb_led_set(colour);
}

int main(void) {
    // Initialize the DHT11 sensor
    dht11_init(DHT11_GPIO_PIN);

    // Initialize the RGB LED
    gpio_init(RGB_LED_PIN, GPIO_OUT);

    while (1) {
        read_dht11();
        rgb_led_control();
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}
