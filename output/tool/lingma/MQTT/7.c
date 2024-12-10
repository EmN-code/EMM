#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/sock/tcp.h"
#include "net/mqtt.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_PUBLISH_INTERVAL (5 * XTIMER_SEC)

static char sendbuf[128];
static char readbuf[128];
static struct mqtt_client client;
static struct sock_tcp_ep broker = { .port = MQTT_BROKER_PORT };
static struct sock_tcp sock;

static void on_puback(void *arg, const struct mqtt_packet *packet)
{
    (void)arg;
    (void)packet;
    printf("PUBACK received\n");
}

static void on_suback(void *arg, const struct mqtt_packet *packet)
{
    (void)arg;
    (void)packet;
    printf("SUBACK received\n");
}

static void on_disconnect(void *arg)
{
    (void)arg;
    printf("Disconnected from broker\n");
}

static void on_publish(void *arg, const struct mqtt_packet *packet)
{
    (void)arg;
    (void)packet;
    printf("PUBLISH received\n");
}

static void on_pingresp(void *arg, const struct mqtt_packet *packet)
{
    (void)arg;
    (void)packet;
    printf("PINGRESP received\n");
}

static void on_error(void *arg, enum mqtt_error error)
{
    (void)arg;
    printf("Error: %d\n", error);
}

static void connect_to_broker(void)
{
    struct mqtt_connect_client_info_t ci = {
        .keep_alive = 60,
        .client_id = "esp32-device",
        .will_retain = 0,
        .will_qos = MQTT_QOS_0,
        .will_topic = NULL,
        .will_message = NULL,
        .clean_session = 1,
    };

    broker.addr.ipv4 = inet_addr(MQTT_BROKER_IP);

    if (sock_tcp_connect(&sock, &broker, NULL) < 0) {
        printf("Failed to connect to broker\n");
        return;
    }

    mqtt_init(&client, &sock, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf),
              on_puback, on_suback, on_disconnect, on_publish, on_pingresp, on_error, NULL);

    if (mqtt_connect(&client, &ci) < 0) {
        printf("Failed to connect to MQTT broker\n");
        sock_tcp_disconnect(&sock);
        return;
    }

    printf("Connected to MQTT broker\n");
}

static void publish_state(void)
{
    static int state = 0;
    char state_str[10];

    if (state % 2 == 0) {
        strcpy(state_str, "work");
    } else {
        strcpy(state_str, "not work");
    }

    struct mqtt_topic_data_t topic_data = {
        .topic = MQTT_TOPIC,
        .qos = MQTT_QOS_0,
        .message = state_str,
    };

    if (mqtt_publish(&client, &topic_data) < 0) {
        printf("Failed to publish message\n");
    } else {
        printf("Published state: %s\n", state_str);
    }

    state++;
}

int main(void)
{
    connect_to_broker();

    while (1) {
        if (mqtt_yield(&client, MQTT_PUBLISH_INTERVAL) < 0) {
            printf("MQTT yield failed\n");
            continue;
        }

        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}