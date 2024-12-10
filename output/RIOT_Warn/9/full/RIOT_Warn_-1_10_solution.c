#include "ztimer.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "sys/socket.h"
#include "stdatomic.h"
#include "unistd.h"
#include "time_units.h"
#include "thread.h"
#include "timex.h"
#include "ztimer.h"
#include "periph/i2c.h"
#include "netinet/in.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "msg.h"
#include "paho_mqtt.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mutex.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "gpio.h"
#include "esp_system.h"

#define THRESHOLD_TEMP 30
#define READ_INTERVAL (5U * 1000U * 1000U)  // 5 seconds in microseconds
#define DHT_PIN GPIO_PIN(0,0)
#define LED_PIN GPIO_PIN(0,1)

void setup_gpio(void) {
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);
}

void read_dht_sensor(void *arg) {
    (void)arg;
    setup_gpio();
    dht_t dev;
    // Assume the DHT sensor is initialized on certain GPIO pin
    dht_init(&dev, DHT_PIN, DHT_TYPE_DHT11);
    int16_t temperature, humidity;
    
    while (1) {
        int res = dht_read(&dev, &temperature, &humidity);
        if (res == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
            
            if (temperature > (THRESHOLD_TEMP * 10)) { // convert threshold to DHT format
                gpio_set(LED_PIN);  // Turn on LED
            } else {
                gpio_clear(LED_PIN);  // Turn off LED
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        ztimer_usleep(ZTIMER_USEC, READ_INTERVAL);
    }
}

int main(void) {
    // Initialize and start a thread that reads the DHT sensor
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, read_dht_sensor, NULL, "dht_reader");
    
    return 0;
}