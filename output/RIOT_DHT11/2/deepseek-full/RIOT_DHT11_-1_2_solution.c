
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "periph/rtc.h"
#include "event/thread.h"
#include "thread.h"

#define DHT11_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000000UL) // 5 seconds in microseconds

static void read_dht11(void)
{
    // Dummy function to simulate DHT11 reading
    // Replace this with actual DHT11 reading logic
    printf("DHT11 reading simulated\n");
}

int main(void)
{
    // Initialize GPIO for DHT11
    gpio_init(DHT11_PIN, GPIO_OUT);

    while (1) {
        // Read DHT11
        read_dht11();

        // Sleep for 5 seconds
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
