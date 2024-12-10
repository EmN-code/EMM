
#include "dev/gpio-hal.h"
#include "sys/timer.h"
#include "sys/etimer.h"
#include "stdio.h"

#define DHT11_PIN 2
#define INTERVAL 5000

void read_dht11(void) {
    // Simulate reading DHT11 data
    printf("Reading DHT11 data from GPIO %d\n", DHT11_PIN);
}

int main(void) {
    gpio_hal_init();

    while (1) {
        read_dht11();
        etimer_set(&et, INTERVAL);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    }

    return 0;
}
