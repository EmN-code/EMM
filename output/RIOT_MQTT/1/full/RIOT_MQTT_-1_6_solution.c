#include <ztimer.h>
#include <timex.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <string.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <arpa/inet.h>
#include <assert.h>
#include <mutex.h>
#include <dht_params.h>
#include <xtimer.h>
#include <time_units.h>
#include <dht.h>
#include <msg.h>
#include <thread.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define STACK_SIZE_THREAD (1024 * 4)
#define MSG_TYPE_PUBLISH (0x1234)
#define INTERVAL (5U * 1000U * 1000U) // 5 seconds in microseconds

kernel_pid_t mqtt_thread_pid;
static char mqtt_stack[STACK_SIZE_THREAD];

static const char *state_topic = "state";
static const char *state_message = "work";

void *mqtt_thread(void *arg) {
    (void)arg;

    sock_udp_t sock;
    sock_udp_ep_t remote = { .port = SERVER_PORT };

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: Unable to parse server IP\n");
        return NULL;
    }

    if (sock_udp_create(&sock, NULL, NULL, 0) != 0) {
        printf("Error: Unable to create UDP socket\n");
        return NULL;
    }

    while (1) {
        ssize_t res = sock_udp_send(&sock, state_message, strlen(state_message), &remote);

        if (res < 0) {
            printf("Error: Unable to send data\n");
        } else {
            printf("Message sent: %s\n", state_message);
        }

        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
    }

    return NULL;
}

int main(void) {
    puts("MQTT client starting\n");

    mqtt_thread_pid = thread_create(mqtt_stack, sizeof(mqtt_stack), THREAD_PRIORITY_MAIN + 1,
                                    THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");

    if (mqtt_thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error: Unable to create MQTT thread\n");
        return 1;
    }

    return 0;
}