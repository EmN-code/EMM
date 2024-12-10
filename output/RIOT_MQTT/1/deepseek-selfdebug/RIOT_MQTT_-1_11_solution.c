
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
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_LARGE)
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static sock_tcp_t mqtt_sock;
static mqtt_client_t mqtt_client;
static char mqtt_send_buf[128];
static char mqtt_read_buf[128];

static void mqtt_connect(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    int res = sock_tcp_connect(&mqtt_sock, &remote, &local, SOCK_FLAGS_REUSE_EP);
    if (res < 0) {
        printf("Error connecting to MQTT broker: %d\n", res);
        return;
    }

    mqtt_client_init(&mqtt_client, &mqtt_sock, mqtt_send_buf, sizeof(mqtt_send_buf), mqtt_read_buf, sizeof(mqtt_read_buf));

    mqtt_connect_info_t conn_info = {
        .client_id = "riot_esp32",
        .keep_alive = MQTT_KEEPALIVE,
        .clean_session = 1,
    };

    res = mqtt_connect(&mqtt_client, &conn_info);
    if (res < 0) {
        printf("Error connecting to MQTT broker: %d\n", res);
        return;
    }
}

static void mqtt_publish(void)
{
    mqtt_publish_info_t pub_info = {
        .topic = MQTT_TOPIC,
        .payload = MQTT_MESSAGE,
        .payload_len = strlen(MQTT_MESSAGE),
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN,
    };

    int res = mqtt_publish(&mqtt_client, &pub_info);
    if (res < 0) {
        printf("Error publishing message: %d\n", res);
    } else {
        printf("Message published successfully\n");
    }
}

int main(void)
{
    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
