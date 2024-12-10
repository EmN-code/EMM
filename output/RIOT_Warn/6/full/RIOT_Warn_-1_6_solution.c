#include "paho_mqtt.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "thread.h"
#include "dht_params.h"
#include "dht.h"

#define TEMP_THRESHOLD 30 // Temperature threshold for LED warning in degrees Celsius
#define DHT_PIN GPIO_PIN(PORT_A, 0)
#define LED_PIN GPIO_PIN(PORT_A, 1)

dht_t dht_sensor;
gpio_t led = LED_PIN;

void *temp_monitor(void *arg) {
    int16_t temperature, humidity;
    int rc;
    // Initialize the DHT sensor
    while (1) {
        rc = dht_read(&dht_sensor, &temperature, &humidity);
        if (rc != 0) {
            printf("Error reading DHT sensor\n");
            continue;
        }

        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature/10, abs(temperature%10), humidity/10, abs(humidity%10));

        if (temperature > TEMP_THRESHOLD * 10) {
            gpio_set(led);
            printf("Warning: Temperature threshold exceeded!\n");
        } else {
            gpio_clear(led);
        }

        rtc_sleep(RTC_OSC_32KHZ, 5000000);  // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    gpio_init(led, GPIO_OUT);
    dht_sensor.params = DHT_PARAM; // assumed initialization method
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, temp_monitor, NULL, "temp_monitor");
    return 0;
}
