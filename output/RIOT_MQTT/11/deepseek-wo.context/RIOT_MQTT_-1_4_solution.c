
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_KEEPALIVE 60
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define MQTT_YIELD_POLLING_MS (30)

static char mqtt_thread_stack[MQTT_THREAD_STACKSIZE];
static mqtt_client_t mqtt_client;
static uint8_t sendbuf[1024];
static uint8_t readbuf[1024];

static void mqtt_publish(void)
{
    mqtt_msg_t msg;
    mqtt_msg_publish_init(&msg);
    msg.topic = MQTT_TOPIC;
    msg.payload = MQTT_MESSAGE;
    msg.payload_len = strlen(MQTT_MESSAGE);
    msg.qos = MQTT_QOS;
    msg.retain = 0;

    mqtt_client_publish(&mqtt_client, &msg);
}

static void mqtt_thread(void)
{
    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }
}

int main(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_addr_from_str(remote.addr.ipv4, MQTT_BROKER_IP);

    sock_tcp_t sock;
    sock_tcp_create(&sock, &local, &remote, SOCK_FLAGS_REUSE_EP);

    mqtt_client_t client;
    mqtt_client_init(&client, &sock, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf), NULL);

    mqtt_connect_t conn;
    mqtt_connect_init(&conn);
    conn.keep_alive = MQTT_KEEPALIVE;

    mqtt_client_connect(&client, &conn);

    thread_create(mqtt_thread_stack, sizeof(mqtt_thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}
