
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/tcp.h>
#include <net/sock/util.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_LARGE)
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MQTT_YIELD_POLLING_MS (30)
#define TIMER_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static sock_tcp_t sock;
static char stack[MQTT_THREAD_STACKSIZE];

void publish_state(void) {
    char message[64];
    sprintf(message, "{\"state\":\"%s\"}", MQTT_MESSAGE);

    sock_tcp_send(sock, message, strlen(message));
}

void mqtt_thread(void) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&sock, &remote, &local, 0) < 0) {
        puts("Error: could not connect to MQTT broker");
        return;
    }

    xtimer_t timer;
    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        if (xtimer_msg_receive_timeout(&timer, TIMER_INTERVAL) >= 0) {
            publish_state();
        }
    }
}

int main(void) {
    thread_create(stack, sizeof(stack), MQTT_THREAD_PRIORITY, 0, (thread_task_func_t)mqtt_thread, NULL, "mqtt_thread");

    return 0;
}
