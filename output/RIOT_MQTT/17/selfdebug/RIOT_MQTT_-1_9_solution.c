#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "msg.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/af.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_TOPIC "device/state"
#define MQTT_PUBLISH_INTERVAL (5U * 1000000U) // 5 seconds

static void _timer_cb(void *arg)
{
    (void)arg;
    printf("Publishing state to the MQTT broker...\n");
    // Publish the state
}

int main(void)
{
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        // Work status can be toggled or determined here and published
        _timer_cb(NULL);

        xtimer_periodic_wakeup(&last_wakeup, MQTT_PUBLISH_INTERVAL);
    }

    return 0;
}