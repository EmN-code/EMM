#include "ztimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "thread_arch.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "paho_mqtt.h"
#include "mbox.h"
#include "msg.h"
#include "mutex.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include "malloc.h"
#include "esp_system.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"
#include "cstring.h"
#include <MQTTClient.h>
#include <arpa/inet.h>

#define GPIO_PIN 2
#define DHT_TYPE DHT11
#define READ_INTERVAL (5U * 1000000U)  // 5 seconds

void read_dht(void) {
    dht_t dev;
    int16_t temp, hum;
    
    // Initialize the DHT sensor
dht_init(&dev, GPIO_PIN, DHT_TYPE);

    // Read the sensor data
    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        // Wait 5 seconds before next read
        ztimer_sleep(ZTIMER_USEC, READ_INTERVAL);
    }
}

int main(void) {
    // Initialize GPIO pin for DHT sensor
gpio_init(GPIO_PIN, GPIO_IN);
    printf("Starting DHT11 reader\n");

    // Start reading from DHT11
    read_dht();
    
    return 0;
}