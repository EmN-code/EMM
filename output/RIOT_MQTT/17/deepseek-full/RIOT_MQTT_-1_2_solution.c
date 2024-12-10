
#include <stdio.h>
#include <string.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <thread.h>
#include <msg.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MQTT_STACK_SIZE (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char mqtt_stack[MQTT_STACK_SIZE];
static msg_t mqtt_msg_queue[8];

void mqtt_publish_thread(void) {
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    char buf[128];
    while (1) {
        snprintf(buf, sizeof(buf), "PUBLISH %s %s", MQTT_TOPIC, MQTT_PAYLOAD);
        if (sock_udp_send(&sock, buf, strlen(buf), &remote) < 0) {
            puts("Error sending MQTT message");
        } else {
            puts("MQTT message sent");
        }
        thread_sleep(PUBLISH_INTERVAL);
    }
}

int main(void) {
    msg_init_queue(mqtt_msg_queue, 8);
    thread_create(mqtt_stack, sizeof(mqtt_stack), MQTT_THREAD_PRIORITY, 0,
                  (thread_task_func_t)mqtt_publish_thread, NULL, "mqtt_publish");

    return 0;
}
