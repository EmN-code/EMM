
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <thread.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define MQTT_THREAD_PRIO (THREAD_PRIORITY_MAIN - 1)

static char mqtt_thread_stack[MQTT_THREAD_STACKSIZE];
static mqtt_client_t mqtt_client;
static sock_tcp_t sock;

void mqtt_publish_thread(void *arg)
{
    (void)arg;
    char payload[20];
    while (1) {
        strcpy(payload, MQTT_MESSAGE);
        mqtt_publish(&mqtt_client, MQTT_TOPIC, payload, strlen(payload), 0, 0);
        xtimer_sleep(5);
    }
}

int main(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_addr_from_str(remote.addr.ipv4, MQTT_BROKER_IP);

    if (sock_tcp_create(&sock, &local, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error creating TCP socket");
        return 1;
    }

    mqtt_client_t client;
    mqtt_client_init(&client, &sock);

    mqtt_connect_t connect;
    connect.keep_alive = 60;
    connect.clean_session = 1;
    connect.client_id = "riot_esp32";
    connect.username = NULL;
    connect.password = NULL;
    connect.will_topic = NULL;
    connect.will_message = NULL;
    connect.will_qos = 0;
    connect.will_retain = 0;

    if (mqtt_connect(&client, &connect) < 0) {
        puts("Error connecting to MQTT broker");
        return 1;
    }

    thread_create(mqtt_thread_stack, sizeof(mqtt_thread_stack), MQTT_THREAD_PRIO,
                  THREAD_CREATE_STACKTEST, mqtt_publish_thread, NULL, "mqtt_publish");

    return 0;
}
