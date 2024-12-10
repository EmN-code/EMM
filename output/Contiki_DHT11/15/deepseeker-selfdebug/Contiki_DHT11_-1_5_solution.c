
#include "board.h"
#include "periph/gpio.h"
#include "xtimer.h"
#include "dht11_sensor.h"

#define DHT11_PIN GPIO_PIN(0, 2)  // GPIO 2 on port 0
#define INTERVAL (5 * 1000 * 1000U)  // 5 seconds in microseconds

static void read_dht11(void)
{
    int16_t temperature, humidity;
    int status = dht11_read(DHT11_PIN, &temperature, &humidity);

    if (status == DHT11_STATUS_OKAY) {
        printf("Temperature: %d.%d C, Humidity: %d.%d %%\n",
               temperature / 10, temperature % 10,
               humidity / 10, humidity % 10);
    } else {
        printf("Failed to read DHT11 sensor, status: %d\n", status);
    }
}

int main(void)
{
    board_init();  // Initialize the board
    gpio_init(DHT11_PIN, GPIO_OUT);  // Initialize GPIO pin for DHT11

    while (1) {
        read_dht11();
        xtimer_usleep(INTERVAL);  // Sleep for 5 seconds
    }

    return 0;
}
