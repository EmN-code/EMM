#include "netstack.h"
#include "routing/routing.h"
#include "dev/leds.h"
#include <stdio.h>

#define DHT11_PORT 1
#define READ_INTERVAL 5  // Read interval in seconds

void configure_dht11(void) {
    // Code to configure the DHT11 sensor
    printf("DHT11 sensor configured\n");
}

void read_dht11(void) {
    // Code to read from the DHT11 sensor
    printf("Reading from DHT11 sensor\n");
    int temperature = 25;  // Dummy temperature value
    int humidity = 60;     // Dummy humidity value
    
    printf("Temperature: %d C, Humidity: %d\n", temperature, humidity);
}

void turn_on_rgb_led(void) {
    // Code to turn on RGB LED
    printf("RGB LED turned on\n");
}

int main(void) {
    configure_dht11();
    while (1) {
        read_dht11();
        turn_on_rgb_led();
        
        // Wait for READ_INTERVAL seconds
        printf("Waiting for %d seconds\n", READ_INTERVAL);
        sleep(READ_INTERVAL);
    }
    return 0;
}