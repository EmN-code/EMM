
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_WILL_TOPIC NULL
#define MQTT_WILL_MESSAGE NULL
#define MQTT_WILL_QOS 0
#define MQTT_WILL_RETAIN 0
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static mqtt_client_t client;
static sock_tcp_t sock;
static char rx_buf[128];
static char tx_buf[128];

void mqtt_event_handler(mqtt_client_t *client, mqtt_event_t event, void *data)
{
    (void)client;
    (void)data;

    switch (event) {
        case MQTT_EVENT_CONNECT:
            printf("MQTT connected\n");
            break;
        case MQTT_EVENT_DISCONNECT:
            printf("MQTT disconnected\n");
            break;
        case MQTT_EVENT_PUBLISH:
            printf("MQTT publish\n");
            break;
        default:
            break;
    }
}

int main(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_addr_t broker_addr;

    if (inet_aton(MQTT_BROKER_IP, &broker_addr) == 0) {
        printf("Invalid broker IP address\n");
        return 1;
    }
    remote.addr.ipv4 = broker_addr;

    if (sock_tcp_create(&sock, &local, &remote, 0) < 0) {
        printf("Failed to create TCP socket\n");
        return 1;
    }

    mqtt_client_config_t config = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = MQTT_KEEPALIVE,
        .will_topic = MQTT_WILL_TOPIC,
        .will_message = MQTT_WILL_MESSAGE,
        .will_qos = MQTT_WILL_QOS,
        .will_retain = MQTT_WILL_RETAIN,
        .username = MQTT_USERNAME,
        .password = MQTT_PASSWORD,
    };

    mqtt_client_init(&client, &sock, &config, mqtt_event_handler);

    if (mqtt_client_connect(&client) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL_MS);

        if (mqtt_client_publish(&client, MQTT_TOPIC, MQTT_MESSAGE, strlen(MQTT_MESSAGE), MQTT_QOS, MQTT_RETAIN) != 0) {
            printf("Failed to publish message\n");
        } else {
            printf("Published message: %s\n", MQTT_MESSAGE);
        }
    }

    return 0;
}
