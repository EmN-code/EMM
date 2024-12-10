
#include <periph/gpio.h>
#include <unistd.h>
#include <stdio.h>

#define DHT11_PIN GPIO_PIN(0, 2)

void read_dht11(void) {
    // Simulate reading DHT11 data
    int temperature = 25; // Example temperature value
    printf("Temperature: %d°C\n", temperature);
}

int main(void) {
    // Initialize GPIO pin for DHT11
    gpio_init(DHT11_PIN, GPIO_OUT);

    while (1) {
        read_dht11();
        sleep(5); // Sleep for 5 seconds
    }

    return 0;
}
