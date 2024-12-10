#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <kernel_defines.h>
#include <msg.h>
#include <dht_params.h>
#include <dht.h>
#include <gpio.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <xtimer.h>
#include <periph/rtc.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mbox.h>
#include <unistd.h>
#include <event/thread.h>
#include <timex.h>
#include <time_units.h>
#include <periph/i2c.h>
#include <periph/gpio.h>

#define THRESHOLD_TEMP 30 // Temperature threshold in degree Celsius
#define DHT_PIN GPIO_PIN(PORT_B, 5) // Define the GPIO pin where DHT11 is connected
#define LED_PIN GPIO_PIN(PORT_B, 0) // Define the GPIO pin where LED is connected

dht_t dev;

void init_sensors()
{
    while (dht_init(&dev, DHT_MODEL_DHT11, DHT_PIN) != DHT_OK) {
        printf("DHT: Initialization failed\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    printf("DHT: Initialized successfully\n");
}

void read_sensor_and_control_led(void *arg)
{
    (void)arg;

    int16_t temperature, humidity;

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
            if (temperature / 10 > THRESHOLD_TEMP) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("DHT: Failed to read data\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000); // Wait for 5 seconds before the next read
    }
}

int main(void)
{
    // Initialize GPIO for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the sensors
    init_sensors();

    // Start the sensor reading and LED control loop
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, read_sensor_and_control_led, NULL, "dht_thread");
    
    return 0;
}
