#include "xtimer.h"
#include "net/sock/util.h"
#include "stdatomic.h"
#include "MQTTClient.h"
#include "net/gcoap.h"
#include "evtimer_mbox.h"
#include "netinet/in.h"
#include "paho_mqtt.h"
#include "stdbool.h"
#include "ztimer.h"
#include "msg.h"
#include "esp_system.h"
#include "timex.h"
#include "stddef.h"
#include "stdio.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "sys/socket.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "architecture.h"
#include "mutex.h"
#include "net/emcute.h"
#include "stdlib.h"
#include "thread.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "unistd.h"
#include "event/thread.h"

#define INTERVAL (1U * US_PER_SEC)
#define LED_PIN GPIO_PIN(PORT_A, 5)

static void cb(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void)
{
    gpio_init(LED_PIN, GPIO_OUT);

    ztimer_t timer;
    timer.callback = cb;
    timer.arg = NULL;

    while (1) {
        ztimer_set(ZTIMER_MSEC, &timer, INTERVAL);
    }

    return 0;
}