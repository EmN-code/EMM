
#include <stdio.h>
#include <thread.h>
#include <dev/gpio-hal.h>
#include <drivers/dht11.h>

#define DHT11_PIN GPIO_PIN(0, 15)  // Example GPIO pin for DHT11
#define RGB_LED_RED GPIO_PIN(0, 16)  // Example GPIO pin for RGB LED Red
#define RGB_LED_GREEN GPIO_PIN(0, 17)  // Example GPIO pin for RGB LED Green
#define RGB_LED_BLUE GPIO_PIN(0, 18)  // Example GPIO pin for RGB LED Blue

#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_YELLOW  (RGB_LED_RED | RGB_LED_GREEN)
#define RGB_LED_CYAN    (RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)

void read_dht11_and_set_led(void) {
    int temperature, humidity;
    while (1) {
        if (dht11_read(DHT11_PIN, &temperature, &humidity) == 0) {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
            // Turn on the RGB LED based on the temperature reading
            if (temperature < 20) {
                gpio_set(RGB_LED_BLUE);
                gpio_clear(RGB_LED_RED | RGB_LED_GREEN);
            } else if (temperature >= 20 && temperature < 30) {
                gpio_set(RGB_LED_GREEN);
                gpio_clear(RGB_LED_RED | RGB_LED_BLUE);
            } else {
                gpio_set(RGB_LED_RED);
                gpio_clear(RGB_LED_GREEN | RGB_LED_BLUE);
            }
        } else {
            printf("Failed to read DHT11 sensor\n");
        }
        thread_sleep(5 * 1000 * 1000);  // Sleep for 5 seconds
    }
}

int main(void) {
    gpio_init(DHT11_PIN, GPIO_OUT);
    gpio_init(RGB_LED_RED, GPIO_OUT);
    gpio_init(RGB_LED_GREEN, GPIO_OUT);
    gpio_init(RGB_LED_BLUE, GPIO_OUT);

    thread_create(NULL, 1024, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, read_dht11_and_set_led, NULL, "dht11_led");

    return 0;
}
