#include <paho_mqtt.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <MQTTClient.h>
#include <event/thread.h>
#include <dht_params.h>
#include <dht.h>
#include <thread.h>

#define THRESHOLD_TEMP 30 // Threshold temperature
#define DHT_PIN 4         // GPIO pin where the DHT11 is connected
#define LED_PIN 2         // GPIO pin where the LED is connected

void *dht_thread(void *arg)
{
    (void)arg;
    dht_t dev;
    int16_t temp, hum;
    // Initialize the DHT11 sensor
    dht_init(&dev, &dht_params[DHT_PIN]);
    
    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
            
            // Check if the temperature exceeds threshold
            if (temp > THRESHOLD_TEMP) {
                gpio_set(LED_PIN); // Turn on LED
            } else {
                gpio_clear(LED_PIN); // Turn off LED
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        // Sleep for 5 seconds
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void)
{
    gpio_init(LED_PIN, GPIO_OUT);
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, dht_thread, NULL, "dht_thread");
    return 0;
}
