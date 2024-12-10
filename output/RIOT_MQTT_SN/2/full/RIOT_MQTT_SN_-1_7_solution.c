#include "net/sock/util.h"
#include "MQTTClient.h"
#include "stdatomic.h"
#include "dht_params.h"
#include "thread.h"
#include "net/gcoap.h"
#include "xtimer.h"
#include "net/ipv6/addr.h"
#include "esp_wifi.h"
#include "stdio.h"
#include "stdbool.h"
#include "timex.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "stdlib.h"
#include "mma8x5x.h"
#include "time_units.h"
#include "mma8x5x_regs.h"
#include "msg.h"
#include "string.h"
#include "arpa/inet.h"
#include "paho_mqtt.h"
#include "dht.h"
#include "ztimer.h"
#include "assert.h"
#include "stddef.h"
#include "mutex.h"

#define EMCUTE_PORT         (1883U)
#define EMCUTE_ID          ("embClient")
#define EMCUTE_PRIO        (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL; // This should never be reached
}

int main(void) {
    static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    msg_init_queue(queue, ARRAY_SIZE(queue));

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");
    
    emcute_con(&gw, true, NULL, NULL, 0, 0);
    
    emcute_topic_t t;
    t.name = "example/topic";
    emcute_reg(&t);

    while(1) {
        
        emcute_pub(&t, (void*)"work", strlen("work"), 0);

        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
