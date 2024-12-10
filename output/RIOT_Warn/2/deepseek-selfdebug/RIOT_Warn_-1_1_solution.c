
#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN             GPIO_PIN(0, 18)  // GPIO pin for DHT11
#define LED_PIN             GPIO_PIN(0, 21)  // GPIO pin for LED
#define TEMP_THRESHOLD      25              // Temperature threshold in Celsius
#define INTERVAL            5               // Interval in seconds

dht_t dht;
dht_params_t dht_params = { .pin = DHT_PIN };

void init_dht(void) {
    if (dht_init(&dht, &dht_params) != DHT_OK) {
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

void read_dht_and_check_temp(void) {
    int16_t temp, hum;
    if (dht_read(&dht, &temp, &hum) != DHT_OK) {
        puts("Failed to read from DHT11");
        return;
    }

    printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

    if (temp / 10 > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);  // Turn on LED
        puts("Temperature exceeds threshold! LED ON");
    } else {
        gpio_clear(LED_PIN);  // Turn off LED
        puts("Temperature within threshold. LED OFF");
    }
}

int main(void) {
    init_dht();
    init_led();

    while (1) {
        read_dht_and_check_temp();
        xtimer_sleep(INTERVAL);
    }

    return 0;
}
