#include <xtimer.h>
#include <periph/gpio.h>

#define DHT11_PIN GPIO_PIN(PORT_A, 2) // example GPIO
#define LED_PIN GPIO_PIN(PORT_B, 6)   // example GPIO
#define THRESHOLD_TEMPERATURE 30      // threshold in degree Celsius

xtimer_ticks32_t last_wakeup;

void dht11_read_temperature(void) {
    int16_t temperature;
    // Mocked function to read temperature from DHT11
    // Replace with actual DHT11 read functionality
    temperature = 25; // assume 25 degrees as a placeholder

    if (temperature > THRESHOLD_TEMPERATURE) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
    
    printf("Temperature: %d\n", temperature);
}

int main(void) {
    gpio_init(DHT11_PIN, GPIO_IN);
    gpio_init(LED_PIN, GPIO_OUT);

    last_wakeup = xtimer_now();
    
    while (1) {
        dht11_read_temperature();
        xtimer_periodic_wakeup(&last_wakeup, 5 * US_PER_SEC);
    }
    
    return 0;
}