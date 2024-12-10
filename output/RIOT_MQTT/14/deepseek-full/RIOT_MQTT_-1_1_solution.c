
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0
#define MQTT_RETAIN 0

#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t sock;
static mqtt_client_t client;
static char sendbuf[128];
static char readbuf[128];

static void publish_state(void)
{
    static int state = 0;
    const char *states[] = {"work", "not work"};

    mqtt_publish_params_t pub_params = {
        .topic = MQTT_TOPIC,
        .message = states[state],
        .msg_len = strlen(states[state]),
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN,
    };

    mqtt_publish(&client, &pub_params);

    state = !state; // Toggle state
}

static void mqtt_event_handler(mqtt_client_t *client, mqtt_event_t event, void *data)
{
    (void)client;
    (void)data;

    switch (event) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT connected\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT disconnected\n");
            break;
        case MQTT_EVENT_PUBLISHED:
            printf("MQTT message published\n");
            break;
        default:
            break;
    }
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

    // Set MQTT event handler
    mqtt_set_event_handler(&client, mqtt_event_handler);

    // Connect to MQTT broker
    mqtt_connect_params_t conn_params = {
        .host = MQTT_BROKER_IP,
        .port = MQTT_BROKER_PORT,
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = MQTT_KEEPALIVE,
        .clean_session = 1,
        .sendbuf = sendbuf,
        .sendbuf_size = sizeof(sendbuf),
        .readbuf = readbuf,
        .readbuf_size = sizeof(readbuf),
    };

    if (mqtt_connect(&client, &conn_params) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
