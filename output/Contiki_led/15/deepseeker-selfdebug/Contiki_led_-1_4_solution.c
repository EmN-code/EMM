
#include "xtimer.h"
#include "dht11.h"
#include "periph/gpio.h"
#include "rgb_led.h"

#define DHT11_PIN GPIO_PIN(0, 15)
#define RGB_LED_PIN GPIO_PIN(0, 16)

void read_dht11(void) {
    int16_t temp, hum;
    if (dht11_read(DHT11_PIN, &temp, &hum) == DHT11_OK) {
        printf("Temperature: %d C, Humidity: %d %%\n", temp, hum);
        rgb_led_set(RGB_LED_WHITE);
    } else {
        printf("Failed to read DHT11 sensor\n");
        rgb_led_set(RGB_LED_RED);
    }
}

int main(void) {
    gpio_init(RGB_LED_PIN, GPIO_OUT);
    dht11_init(DHT11_PIN);

    while (1) {
        read_dht11();
        xtimer_sleep(5);
    }

    return 0;
}
