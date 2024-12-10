#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define EMCUTE_ID_LEN    64U   /*  Address length for the connection */
#define EMCUTE_PORT      1883  /*  Port for the MQTT-SN connection */

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

void *mqtt_thread(void *arg) {
    (void) arg;
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    /* Initialize the MQTT-SN client */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to MQtt-SN gateway\n");
        return NULL;
    }
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;
    t.name = "device/status";
    emcute_reg(&t);
    while (1) {
        /* Publish the state every 5 seconds */
        char *data = "work";
        if (emcute_pub(&t, data, strlen(data), flags) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        }
        sleep(5);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);
    puts("MQTT-SN example application\n");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0,
                            mqtt_thread, NULL, "mqtt_thread");
    return 0;
}