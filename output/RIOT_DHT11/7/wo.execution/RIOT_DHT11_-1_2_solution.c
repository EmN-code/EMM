#include <thread_arch.h>
#include <thread.h>
#include <event/thread.h>
#include <dht.h>

#define DHT11_PIN 2
#define THREAD_STACKSIZE_MAIN 1024

static char stack[THREAD_STACKSIZE_MAIN];

void *dht11_thread(void *arg) {
    (void)arg;
    while (1) {
        int16_t temperature, humidity;

        if (dht_read(&temperature, &humidity, DHT11_PIN) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        }
        else {
            printf("Failed to read from DHT11 sensor\n");
        }

        thread_sleep(5);
    }
    return NULL;
}

int main(void) {
    dht_init();

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, dht11_thread, NULL, "dht11_thread");

    return 0;
}
