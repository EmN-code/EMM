#include "periph/gpio.h"
#include "periph/i2c.h"
#include "timex.h"
#include "ztimer.h"

#define LED_PIN GPIO_PIN(PORT_A, 0)  // Assuming the LED is connected to PORT_A, pin 0
#define DHT_PIN GPIO_PIN(PORT_B, 1)  // Assuming the DHT is connected to PORT_B, pin 1
#define TEMPERATURE_THRESHOLD 30

void read_dht11(void) {
    int temperature = 0; // Placeholder for temperature reading logic
    // Implement the logic to read temperature from DHT11

    if (temperature > TEMPERATURE_THRESHOLD) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_init(DHT_PIN, GPIO_IN);

    while (1) {
        read_dht11();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}