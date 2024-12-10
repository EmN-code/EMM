
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t sock;
static mqtt_client_t mqtt_client;
static char sendbuf[128];
static char readbuf[128];

void mqtt_connect(void) {
    mqtt_client_t *client = &mqtt_client;
    mqtt_client_init(client, NULL, 0, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    mqtt_connect_info_t conn_info = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = 60,
        .clean_session = 1,
        .will_topic = NULL,
        .will_message = NULL,
        .will_qos = 0,
        .will_retain = 0,
        .username = NULL,
        .password = NULL
    };

    int res = MQTTPublish(&mqtt_client, MQTT_TOPIC, &(MQTTMessage){
        .payload = MQTT_MESSAGE,
        .payloadlen = strlen(MQTT_MESSAGE),
        .qos = MQTT_QOS,
        .retained = MQTT_RETAIN
    });

    if (res != 0) {
        printf("MQTT publish failed with error code %d\n", res);
    } else {
        printf("MQTT message published successfully\n");
    }
}

int main(void) {
    // Initialize network interface
    netif_t *netif = netif_iter(NULL);
    if (netif == NULL) {
        printf("No network interface found\n");
        return 1;
    }

    // Initialize UDP socket
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;
    sock_udp_create(&sock, &local, NULL, 0);

    // Connect to MQTT broker
    mqtt_connect();

    // Publish state every 5 seconds
    while (1) {
        xtimer_usleep(PUBLISH_INTERVAL);
        mqtt_connect();
    }

    return 0;
}
