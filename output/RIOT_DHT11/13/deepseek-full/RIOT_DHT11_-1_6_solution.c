
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <periph/gpio.h>
#include <xtimer.h>

#define DHT11_PIN GPIO_PIN(0, 2)
#define INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

void read_dht11(void) {
    int8_t temperature, humidity;
    gpio_t pin = DHT11_PIN;

    // Initialize GPIO pin for DHT11
    if (gpio_init(pin, GPIO_OUT) < 0) {
        puts("Error initializing GPIO pin");
        return;
    }

    // DHT11 start signal
    gpio_clear(pin);
    xtimer_usleep(18000); // 18ms
    gpio_set(pin);
    xtimer_usleep(40); // 40us

    // DHT11 response signal
    gpio_init(pin, GPIO_IN);
    xtimer_usleep(80); // 80us

    // Read DHT11 data
    uint8_t data[5] = {0};
    for (int i = 0; i < 40; i++) {
        while (gpio_read(pin) == 0); // Wait for high
        xtimer_usleep(50); // 50us
        if (gpio_read(pin) == 1) {
            data[i / 8] |= (1 << (7 - (i % 8)));
        }
        while (gpio_read(pin) == 1); // Wait for low
    }

    // Check checksum
    if ((data[0] + data[1] + data[2] + data[3]) == data[4]) {
        temperature = data[2];
        humidity = data[0];
        printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
    } else {
        puts("Checksum error");
    }
}

int main(void) {
    while (1) {
        read_dht11();
        xtimer_usleep(INTERVAL);
    }
    return 0;
}
