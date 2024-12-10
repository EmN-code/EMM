#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>

#define DHT11_PIN GPIO_PIN(PORT_A, 10)
#define LED_PIN GPIO_PIN(PORT_B, 5)
#define TEMP_THRESHOLD 30 // 30 degrees Celsius

void read_dht11(int *temperature, int *humidity) {
    // Simulated read function for the DHT11 sensor
    *temperature = 28; // Example values
    *humidity = 50;
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    int temperature, humidity;
    
    while(1) {
        read_dht11(&temperature, &humidity);
        printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
        
        if (temperature > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
            printf("Warning: Temperature exceeded threshold!\n");
        } else {
            gpio_clear(LED_PIN);
        }

        xtimer_sleep(5); // wait for 5 seconds
    }
    return 0;
}