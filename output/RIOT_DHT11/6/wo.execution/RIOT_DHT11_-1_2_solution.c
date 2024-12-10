#include "thread_flags.h"
#include "thread.h"
#include "nvs_flash.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"

#define DHT_PIN GPIO_NUM_2

static dht_t dev;

// Thread stack size
#define THREAD_STACKSIZE 1024
char stack[THREAD_STACKSIZE];

void *dht_reader_thread(void *arg) {
    (void)arg;
    int16_t temp, hum;
    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Could not read data from DHT11\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    nvs_flash_init();
    dht_params_t my_dht_params = {
        .pin = DHT_PIN,
        .in_mode = DHT_PARAM_MODE, 
        .out_mode = DHT_PARAM_OUT_MODE
    };

    if (dht_init(&dev, &my_dht_params) != 0) {
        printf("Failed to initialize the DHT11 sensor\n");
        return 1;
    }

    // Create the DHT reader thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, dht_reader_thread, NULL, "dht_reader");

    // Let the main thread idle
    thread_sleep();

    return 0;
}