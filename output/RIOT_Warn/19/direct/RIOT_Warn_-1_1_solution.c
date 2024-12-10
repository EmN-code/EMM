#include "periph_conf.h"
#include "mutex.h"
#include "thread.h"
#include "stdio.h"
#include "dht.h"
#include "board.h"

#define DHT11_PIN GPIO_PIN(0, 4)
#define LED_PIN GPIO_PIN(0, 2)
#define TEMP_THRESHOLD 30

static char stack[THREAD_STACKSIZE_MAIN];

void *read_dht11(void *arg) {
    (void)arg;
    int16_t temp, hum;
    while (1) {
        if (dht_read(DHT11_PIN, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d\n", temp);
            printf("Humidity: %d\n", hum);
            
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    puts("DHT11 reading example\n");
    
    gpio_init(LED_PIN, GPIO_OUT);
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, read_dht11, NULL, "dht11_reader");
    
    while (1) {
        thread_yield();
    }

    return 0;
}