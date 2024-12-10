#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kernel_defines.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <thread.h>
#include <mutex.h>
#include <MQTTClient.h>
#include <event/thread.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <sys/socket.h>
#include <dht_params.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define PUBLISH_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static kernel_pid_t sender_pid;
static emcute_topic_t t;
static sock_udp_ep_t remote;

void *mqtt_publisher(void *arg) {
    (void)arg;
    char message[] = "work or not work";
    int res;

    res = emcute_reg(&t);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return NULL;
    }

    while (1) {
        res = emcute_pub(&t, message, sizeof(message), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }
    return NULL;
}

int main(void) {
    puts("Starting RIOT MQTT-SN Client");

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to encode destination address");
        return 1;
    }

    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    t.name = "riot/test";
    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    sender_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_publisher, NULL, "mqtt_publisher");
    if (sender_pid <= KERNEL_PID_UNDEF) {
        puts("Error: unable to create publisher thread");
        return 1;
    }

    return 0;
}