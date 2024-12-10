
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include <net/mqtt_client.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void _on_publish(void *arg, int reason_code)
{
    (void)arg;
    printf("Publish result: %d\n", reason_code);
}

int main(void)
{
    sock_tcp_t sock;
    mqtt_client_t client;
    mqtt_client_config_t config = MQTT_CLIENT_CONFIG_DEFAULT;
    mqtt_client_connect_params_t conn_params = MQTT_CLIENT_CONNECT_PARAMS_DEFAULT;
    mqtt_client_will_t will = { .topic = TOPIC, .message = PAYLOAD, .qos = MQTT_QOS_0, .retain = 0 };
    mqtt_client_publish_params_t pub_params = { .topic = TOPIC, .message = PAYLOAD, .qos = MQTT_QOS_0, .retain = 0 };

    printf("Starting MQTT client example\n");

    /* Initialize the socket */
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&sock, &remote, &local, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error connecting to broker");
        return 1;
    }

    /* Initialize the MQTT client */
    mqtt_client_init(&client, &sock, &config);

    /* Set up connection parameters */
    conn_params.client_id = "riot_client";
    conn_params.clean_session = true;
    conn_params.keep_alive = 60;
    conn_params.will = &will;

    /* Connect to the broker */
    if (mqtt_client_connect(&client, &conn_params) != MQTT_OK) {
        puts("Error connecting to MQTT broker");
        return 1;
    }

    /* Publish a message every 5 seconds */
    while (1) {
        if (mqtt_client_publish(&client, &pub_params, _on_publish, NULL) != MQTT_OK) {
            puts("Error publishing message");
        }
        xtimer_usleep(INTERVAL);
    }

    /* Disconnect from the broker */
    mqtt_client_disconnect(&client);

    return 0;
}
