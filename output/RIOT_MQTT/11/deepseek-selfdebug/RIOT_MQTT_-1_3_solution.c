
#include <stdio.h>
#include <thread.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_CLIENT_ID "esp32_client"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static void *mqtt_thread(void *arg)
{
    (void)arg;

    sock_tcp_t sock;
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_addr_from_str(MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&sock, &remote, &local, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: failed to connect to MQTT broker");
        return NULL;
    }

    mqtt_client_t client;
    mqtt_client_config_t config = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = 60,
        .clean_session = 1,
        .will_topic = NULL,
        .will_message = NULL,
        .will_qos = 0,
        .will_retain = 0,
        .username = NULL,
        .password = NULL,
    };

    if (mqtt_client_init(&client, &config, &sock) < 0) {
        puts("Error: failed to initialize MQTT client");
        sock_tcp_close(&sock);
        return NULL;
    }

    mqtt_publish_t pub = {
        .topic = MQTT_TOPIC,
        .message = MQTT_MESSAGE,
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN,
    };

    while (1) {
        if (mqtt_publish(&client, &pub) < 0) {
            puts("Error: failed to publish message");
        } else {
            puts("Message published successfully");
        }

        xtimer_sleep(5);
    }

    mqtt_client_disconnect(&client);
    sock_tcp_close(&sock);

    return NULL;
}

int main(void)
{
    thread_create(NULL, THREAD_STACKSIZE, THREAD_PRIORITY, 0, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}
