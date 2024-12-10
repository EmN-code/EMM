#include <xtimer.h> // Contains the xtimer_sleep function which is required for delaying
#include <stdio.h>   // For input and output functions
#include <dht.h>     // DHT driver header
#include <periph/gpio.h> // GPIO interaction library

int main(void)
{
    dht_t sensor;
    sensor.type = DHT11; // Specify the sensor type DHT11
    sensor.pin = GPIO_PIN(0, 2); // Set GPIO 2 as the DHT11 data pin
    
    printf("Reading DHT11 on GPIO 2 every 5 seconds\n");

    while (1) {
        int16_t temperature, humidity;
        if (dht_read(&sensor, &temperature, &humidity) != DHT_OK) {
            printf("Failed to read DHT11 sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temperature / 10, temperature % 10, 
                   humidity / 10, humidity % 10);
        }

        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}