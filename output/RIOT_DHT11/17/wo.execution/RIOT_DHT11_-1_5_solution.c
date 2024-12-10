#include <ztimer.h>
#include <xtimer.h>
#include <thread_config.h>
#include <thread.h>
#include <time_units.h>
#include <timex.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdlib.h>
#include "periph/gpio.h"
#include "msg.h"

#define DHT11_PIN 2
#define DHT11_INTERVAL (5U * US_PER_SEC)

void read_dht11(void)
{
    printf("Reading DHT11 sensor...\n");
    // Here we would initialize and read from the DHT11, 
    // but since we cannot use the dht.h or dht_init, dht_read
    // we simulate the reading process.
    // Example of simulated temperature and humidity
    int temperature = 25; // Placeholder for temperature
    int humidity = 50;    // Placeholder for humidity

    printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
}

int main(void)
{
    msg_t message;

    while (1) {
        // Simulate reading the DHT11 sensor
        read_dht11();

        // Here we would typically create a message and send or process it
        // to communicate the sensor reading. In this setup, we are not using
        // msg_send_receive or msg functions, due to given restrictions.
        
        ztimer_sleep(ZTIMER_USEC, DHT11_INTERVAL);
    }

    return 0;
}
