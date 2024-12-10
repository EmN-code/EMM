
#include "periph/gpio.h"
#include "ztimer.h"
#include "fmt.h"

#define DHT11_PIN GPIO_PIN(0, 2)
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void dht11_read(int16_t *temperature, int16_t *humidity) {
    gpio_t pin = DHT11_PIN;
    gpio_init(pin, GPIO_OUT);
    gpio_clear(pin);
    ztimer_sleep(ZTIMER_USEC, 18000);  // 18ms
    gpio_init(pin, GPIO_IN);
    ztimer_sleep(ZTIMER_USEC, 40);  // 40us

    // Read the DHT11 data
    uint8_t data[5];
    for (int i = 0; i < 5; i++) {
        data[i] = 0;
        for (int j = 0; j < 8; j++) {
            while (gpio_read(pin) == 0);
            ztimer_sleep(ZTIMER_USEC, 40);  // 40us
            if (gpio_read(pin) == 1) {
                data[i] |= (1 << (7 - j));
            }
            while (gpio_read(pin) == 1);
        }
    }

    // Check checksum
    if ((uint8_t)(data[0] + data[1] + data[2] + data[3]) == data[4]) {
        *humidity = data[0];
        *temperature = data[2];
    } else {
        *humidity = -1;
        *temperature = -1;
    }
}

int main(void) {
    int16_t temperature, humidity;

    while (1) {
        dht11_read(&temperature, &humidity);

        if (temperature != -1 && humidity != -1) {
            char temp_str[10], hum_str[10];
            fmt_s16_dfp(temp_str, temperature, -1);
            fmt_s16_dfp(hum_str, humidity, -1);
            puts("Temperature: " temp_str " C, Humidity: " hum_str " %");
        } else {
            puts("Failed to read DHT11 sensor");
        }

        ztimer_sleep(ZTIMER_USEC, INTERVAL);
    }

    return 0;
}
