
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
#define MQTT_KEEPALIVE 30
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

#define TIMER_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static mqtt_client_t client;
static sock_tcp_t sock;
static char rx_buf[128];
static char tx_buf[128];

static void _mqtt_connect(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    int res = sock_tcp_connect(&sock, &remote, &local, SOCK_FLAGS_REUSE_EP);
    if (res < 0) {
        printf("Error connecting to MQTT broker: %d\n", res);
        return;
    }

    mqtt_init(&client, &sock, rx_buf, sizeof(rx_buf), tx_buf, sizeof(tx_buf), _mqtt_event);
    mqtt_connect(&client, NULL, NULL, MQTT_KEEPALIVE, MQTT_USERNAME, MQTT_PASSWORD);
}

static void _mqtt_event(mqtt_client_t *client, mqtt_event_t event, void *data)
{
    (void)client;
    (void)data;

    switch (event) {
        case MQTT_EVENT_CONNECT:
            printf("Connected to MQTT broker\n");
            break;
        case MQTT_EVENT_DISCONNECT:
            printf("Disconnected from MQTT broker\n");
            break;
        case MQTT_EVENT_PUBLISH:
            printf("Message published\n");
            break;
        default:
            break;
    }
}

static void _mqtt_publish(void)
{
    mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE, strlen(MQTT_MESSAGE), MQTT_QOS, MQTT_RETAIN);
}

int main(void)
{
    xtimer_ticks32_t last_wakeup = xtimer_now();

    _mqtt_connect();

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
        _mqtt_publish();
    }

    return 0;
}
