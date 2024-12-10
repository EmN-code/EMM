#include <ztimer.h>  // Correct timer header file
#include <stdio.h>
#include <dht.h>     // Ensure the correct DHT header is included
#include <periph/gpio.h>  // Ensure the correct GPIO header is included

// DHT setup
static dht_t dev;
#define DHT_PIN GPIO_PIN(0, 2)  // GPIO Pin 2

static void read_sensor(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d\n",
               temp / 10, temp % 10, hum / 10, hum % 10);
    }
    else {
        puts("Failed to read data from sensor");
    }
}

int main(void) {
    // Initialize the DHT sensor
    // The dht_init function takes two arguments: the device structure pointer and the pin configuration
    dht_init(&dev, DHT_PIN, DHT11);  // Correct number of arguments
    
    // Infinite loop to read sensor data every 5 seconds
    while (1) {
        read_sensor();
        ztimer_sleep(ZTIMER_SEC, 5);  // Use ZTIMER_SEC for 5-second interval
    }

    return 0;
}