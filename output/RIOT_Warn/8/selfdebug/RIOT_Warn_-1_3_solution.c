#include <xtimer.h>
#include <periph/gpio.h>
#include <stdio.h>

#define DHT11_PIN GPIO_PIN(PORT_A, 0) // use a specific pin for the DHT11 sensor
#define LED_PIN GPIO_PIN(PORT_B, 0) // use a specific pin for the LED
#define TEMP_THRESHOLD 30.0 // temperature threshold for turning on the LED

static void read_sensor(void){
    // Simulating DHT11 sensor reading
    float temperature = 25; // replace this with actual sensor value retrieval
    printf("Current temperature: %.1f\n", temperature);
    if(temperature > TEMP_THRESHOLD){
        printf("Warning: Temperature exceeds threshold!\n");
        gpio_set(LED_PIN);
    }
    else{
        gpio_clear(LED_PIN);
    }
}

int main(void){
    gpio_init(LED_PIN, GPIO_OUT);
    while (1){
        read_sensor();
        xtimer_sleep(5);
    }
    return 0;
}