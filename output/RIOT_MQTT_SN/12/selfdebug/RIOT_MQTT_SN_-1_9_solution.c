#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include "net/emcute.h"
#include "net/sock/udp.h"

#define BROKER_ADDRESS "47.102.103.1"
#define BROKER_PORT 1883
#define STATE_TOPIC "device/state"
#define STATE_MESSAGE "work"
#define INTERVAL (5U)

static void *emcute_thread(void *arg) {
    emcute_con_t con;
    sock_udp_ep_t gw = {
        .family = AF_INET6,
        .port = BROKER_PORT
    };
    inet_pton(AF_INET6, BROKER_ADDRESS, &gw.addr.ipv6);
    
    if (emcute_con(&gw, true, NULL, NULL, 0, EMCUTE_QOS_0) != EMCUTE_OK) {
        printf("Connection to MQTT-SN broker failed\n");
        return NULL;
    }
    
    emcute_topic_t t;
    t.name = STATE_TOPIC;
    emcute_reg(&t);
    
    while (1) {
        if (emcute_pub(&t, STATE_MESSAGE, strlen(STATE_MESSAGE), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Publication failed\n");
        }
        sleep(INTERVAL);
    }
    
    return NULL;
}

int main(void) {
    puts("MQTT-SN publisher application\n");
    
    thread_create(emcute_stack, sizeof(emcute_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");
    
    return 0;
}