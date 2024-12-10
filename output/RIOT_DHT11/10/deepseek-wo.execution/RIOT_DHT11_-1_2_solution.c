
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "event/thread.h"
#include "thread.h"

#define DHT11_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void read_dht11(void) {
    int16_t temperature, humidity;
    gpio_t dht11_gpio = DHT11_PIN;

    // Initialize GPIO for DHT11
    gpio_init(dht11_gpio, GPIO_OUT);
    gpio_set(dht11_gpio);

    // Dummy read to stabilize the sensor
    for (int i = 0; i < 20; i++) {
        gpio_clear(dht11_gpio);
        xtimer_usleep(500);
        gpio_set(dht11_gpio);
        xtimer_usleep(500);
    }

    // Read DHT11 data
    gpio_clear(dht11_gpio);
    xtimer_usleep(18000); // Start signal
    gpio_set(dht11_gpio);
    xtimer_usleep(40); // Wait for response

    // Read the response
    gpio_init(dht11_gpio, GPIO_IN);
    if (gpio_read(dht11_gpio) == 0) {
        xtimer_usleep(80); // Wait for data start
        for (int i = 0; i < 40; i++) {
            while (gpio_read(dht11_gpio) == 0); // Wait for high
            uint32_t t = xtimer_now_usec();
            while (gpio_read(dht11_gpio) == 1); // Wait for low
            if ((xtimer_now_usec() - t) > 50) {
                if (i < 16) {
                    humidity |= (1 << (15 - i));
                } else {
                    temperature |= (1 << (31 - i));
                }
            }
        }
    }

    // Convert raw data to temperature and humidity
    humidity >>= 8;
    temperature >>= 16;

    printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
}

int main(void) {
    while (1) {
        read_dht11();
        xtimer_usleep(SLEEP_INTERVAL);
    }
    return 0;
}
