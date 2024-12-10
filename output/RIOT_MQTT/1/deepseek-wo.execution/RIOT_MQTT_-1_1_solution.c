
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/af.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD_WORK "work"
#define PAYLOAD_NOT_WORK "not work"
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t sock;
static mqtt_client_t client;
static char sendbuf[128];
static char readbuf[128];

void publish_state(mqtt_client_t *client, const char *payload)
{
    mqtt_publish_params_t pub_params = {
        .topic = TOPIC,
        .payload = payload,
        .payload_len = strlen(payload),
        .qos = MQTT_QOS_0,
        .retain = false,
        .dup = false
    };

    mqtt_publish(client, &pub_params);
}

int main(void)
{
    // Initialize network interface
    netif_t *netif = netif_iter(NULL);
    if (netif == NULL) {
        puts("No network interface found");
        return 1;
    }

    // Initialize MQTT client
    mqtt_client_init(&client, netif, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to MQTT broker
    mqtt_connect_params_t conn_params = {
        .host = BROKER_IP,
        .port = BROKER_PORT,
        .client_id = "riot_esp32",
        .keep_alive = 60,
        .clean_session = true,
        .will = NULL,
        .username = NULL,
        .password = NULL
    };

    if (mqtt_connect(&client, &conn_params) != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    puts("Connected to MQTT broker");

    // Publish state every 5 seconds
    while (1) {
        publish_state(&client, PAYLOAD_WORK);
        xtimer_usleep(INTERVAL);
        publish_state(&client, PAYLOAD_NOT_WORK);
        xtimer_usleep(INTERVAL);
    }

    // Disconnect from MQTT broker
    mqtt_disconnect(&client);

    return 0;
}
