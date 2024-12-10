#include <ztimer.h>
#include <xtimer.h>
#include <thread_config.h>
#include <thread.h>
#include <timex.h>
#include <time_units.h>
#include <periph/rtc.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mbox.h>
#include <event/timeout.h>
#include <event/thread.h>
#include <event/periodic.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <dht.h>
#include <MQTTClient.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * US_PER_SEC) // 5 seconds

void *dht_thread(void *arg) {
    (void)arg;
    int16_t temp, hum;

    while (1) {
        if (dht_read(&temp, &hum, DHT_PIN) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, abs(temp % 10),
                   hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT11\n");
        }
        xtimer_sleep(SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void) {
    static char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, dht_thread, NULL, "dht");
    return 0;
}