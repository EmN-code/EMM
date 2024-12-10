#include <ztimer.h>
#include <unistd.h>
#include <time_units.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mutex.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <arpa/inet.h>
#include <net/emcute.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <thread.h>
#include <MQTTClient.h>

void *alarm(void *arg) {
    (void)arg;
    ztimer_sleep(ZTIMER_SEC, 5);  // Simulating alarm trigger delay
    printf("Alarm triggered!\n");
    return NULL;
}

int main(void) {
    mutex_t lock;
    mutex_init(&lock);
    mutex_lock(&lock);

    // Simulate setting RTC time to 2024-06-01
    printf("RTC time set to 2024-06-01\n");

    // Thread for the alarm
    thread_t alarm_thread;
    char alarm_thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(alarm_thread_stack, sizeof(alarm_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, 0, alarm, NULL,
                  "alarm thread");

    mutex_unlock(&lock);

    while(true) {
        // Simulate network handling or other tasks
    }

    return 0;
}