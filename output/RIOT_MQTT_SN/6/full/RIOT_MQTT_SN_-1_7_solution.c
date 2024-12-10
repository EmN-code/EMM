#include <ztimer.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <dht_params.h>
#include <dht.h>
#include <msg.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <net/gcoap.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <timex.h>
#include <ztimer.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <mutex.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <kernel_defines.h>
#include <thread.h>
#include <MQTTClient.h>
#include <stddef.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mbox.h>
#include <time_units.h>
#include <net/sock/util.h>

#define EMCUTE_ID           "esp32"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define SERVER_PORT         (1883U)
#define SERVER_ADDR         "47.102.103.1"
#define PUB_INTERVAL        (5U)
#define TOPIC_NAME          "device/state"

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];
static char *here_state = "work";

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(SERVER_PORT, EMCUTE_ID);
    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");
    
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4);

    while (1) {
        int conn_rc = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (conn_rc == EMCUTE_OK) {
            printf("Successfully connected to the MQTT-SN gateway\n");
            break;
        } else {
            printf("Connection attempt failed, retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
        }
    }

    emcute_topic_t t;
    t.name = TOPIC_NAME;
    while (emcute_reg(&t) != EMCUTE_OK) {
        printf("Registering topic failed, retrying...\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    while (1) {
        if (emcute_pub(&t, here_state, sizeof(here_state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Publish failed, reconnecting...\n");
            emcute_con(&gw, true, NULL, NULL, 0, 0);
        }
        ztimer_sleep(ZTIMER_SEC, PUB_INTERVAL);
    }

    return 0;
}
