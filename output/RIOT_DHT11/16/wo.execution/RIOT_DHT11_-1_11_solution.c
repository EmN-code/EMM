#include <ztimer.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>
#include <msg.h>
#include <mutex.h>
#include <timex.h>
#include <unistd.h>
#include <thread.h>
#include <sys/socket.h>
#include <time_units.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mbox.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <arpa/inet.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define DHT_GPIO            2
#define SLEEP_INTERVAL      (5U * US_PER_SEC)

static void *dht_thread(void *arg) {
    (void)arg;
    int16_t temperature, humidity;

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        }
        else {
            puts("Error reading from DHT\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000U);
    }

    return NULL;
}

int main(void) {
    puts("RIOT DHT Example -- Reading DHT11 sensor\n");

    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    kernel_pid_t dht_pid = thread_create(dht_thread_stack, sizeof(dht_thread_stack),
                                         THREAD_PRIORITY_MAIN, 0,
                                         dht_thread, NULL, "dht");
    if (dht_pid <= KERNEL_PID_UNDEF) {
        puts("[Error] Unable to create the DHT thread\n");
        return 1;
    }

    return 0;
}