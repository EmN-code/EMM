
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/af.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t sock;
static mqtt_client_t client;
static mqtt_connect_info_t conn_info;
static char sendbuf[128];
static char readbuf[128];

static void _mqtt_event_cb(mqtt_client_t *client, mqtt_event_t event, void *ctx)
{
    (void)client;
    (void)ctx;

    switch (event) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT connected\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT disconnected\n");
            break;
        default:
            break;
    }
}

static void _publish_state(void)
{
    mqtt_publish_t pub;
    pub.topic = MQTT_TOPIC;
    pub.payload = MQTT_MESSAGE;
    pub.payload_len = strlen(MQTT_MESSAGE);
    pub.qos = MQTT_QOS;
    pub.retain = MQTT_RETAIN;

    mqtt_publish(&client, &pub);
}

int main(void)
{
    // Initialize network interface
    netif_t *netif = netif_iter(NULL);
    if (netif == NULL) {
        printf("No network interface found\n");
        return 1;
    }

    // Initialize MQTT client
    mqtt_client_init(&client);

    // Set up connection info
    conn_info.client_id = MQTT_CLIENT_ID;
    conn_info.keep_alive = 60;
    conn_info.clean_session = 1;
    conn_info.will_topic = NULL;
    conn_info.will_message = NULL;
    conn_info.will_qos = 0;
    conn_info.will_retain = 0;

    // Connect to MQTT broker
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        printf("Failed to create UDP socket\n");
        return 1;
    }

    if (mqtt_connect(&client, &sock, &conn_info, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf), _mqtt_event_cb, NULL) < 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        _publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
