
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 30
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)

static sock_tcp_t mqtt_sock;
static mqtt_client_t mqtt_client;
static char mqtt_thread_stack[MQTT_THREAD_STACKSIZE];

static void mqtt_publish(void)
{
    mqtt_msg_t msg;
    mqtt_msg_publish_init(&msg);
    mqtt_msg_publish_topic(&msg, MQTT_TOPIC);
    mqtt_msg_publish_payload(&msg, MQTT_MESSAGE, strlen(MQTT_MESSAGE));
    mqtt_msg_publish_qos(&msg, MQTT_QOS);
    mqtt_msg_publish_retain(&msg, MQTT_RETAIN);

    mqtt_client_publish(&mqtt_client, &msg);
}

static void *mqtt_thread(void *arg)
{
    (void)arg;

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void)
{
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);
    remote.port = MQTT_BROKER_PORT;

    sock_tcp_create(&mqtt_sock, &local, &remote, 0);

    mqtt_client_init(&mqtt_client, &mqtt_sock, MQTT_KEEPALIVE);

    thread_create(mqtt_thread_stack, sizeof(mqtt_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  mqtt_thread, NULL, "mqtt_thread");

    return 0;
}
