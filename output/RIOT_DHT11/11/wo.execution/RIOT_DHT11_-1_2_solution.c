#include <thread.h>
#include <event/thread.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT11_PIN GPIO_PIN(0, 2)

static int _read_dht(void) {
    int16_t temp, hum;
    if (dht_read(&temp, &hum, DHT11_PIN) != DHT_OK) {
        printf("Error reading DHT sensor\n");
        return -1;
    }
    printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
           temp / 10, temp % 10, hum / 10, hum % 10);
    return 0;
}

void *dht_thread(void *arg) {
    (void)arg;
    while (1) {
        _read_dht();
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    puts("DHT11 Sensor Reading Example\n");
    thread_create(NULL, -1, THREAD_PRIORITY_MAIN, 0, dht_thread, NULL, "dht");
    return 0;
}