
#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN             GPIO_PIN(0, 18)  // GPIO pin for DHT11
#define LED_PIN             GPIO_PIN(0, 21)  // GPIO pin for LED
#define TEMP_THRESHOLD      30               // Temperature threshold in Celsius
#define INTERVAL            5                // Interval in seconds

static dht_t dht11;
static dht_params_t dht11_params = {
    .pin = DHT_PIN,
    .type = DHT11,
    .in_mode = DHT_PARAM_PULL,
};

void init_dht11(void) {
    if (dht_init(&dht11, &dht11_params) != DHT_OK) {
        puts("DHT11 initialization failed");
        return;
    }
    puts("DHT11 initialized successfully");
}

void init_led(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);  // Ensure LED is off initially
    puts("LED initialized successfully");
}

void read_dht11(int16_t *temp, int16_t *hum) {
    if (dht_read(&dht11, temp, hum) != DHT_OK) {
        puts("Failed to read from DHT11");
        return;
    }
    printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", *temp / 10, *temp % 10, *hum / 10, *hum % 10);
}

void check_temperature(int16_t temp) {
    if (temp / 10 > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);  // Turn on LED
        puts("Temperature exceeds threshold! LED ON");
    } else {
        gpio_clear(LED_PIN);  // Turn off LED
        puts("Temperature within threshold. LED OFF");
    }
}

int main(void) {
    int16_t temp, hum;

    init_dht11();
    init_led();

    while (1) {
        read_dht11(&temp, &hum);
        check_temperature(temp);
        xtimer_sleep(INTERVAL);
    }

    return 0;
}
