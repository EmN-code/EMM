#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mqtt.h>
#include <esp_wifi.h>
#include <net/sntp.h>
#include <net/ipv6/uip-nd6.h>
#include <net/gnrc.h>

#define MQTT_BROKER_ADDRESS "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "esp32/status"
#define MQTT_CLIENT_ID "esp32-client"
#define MQTT_KEEPALIVE_INTERVAL 60

static mqtt_client_t client;
static char inbuf[128];
static char outbuf[128];

static void on_pub(void *arg, const char *topic, size_t topic_len, const void *payload, size_t payload_len)
{
    (void)arg;
    printf("[Received] Topic: %.*s, Payload: %.*s\n", (int)topic_len, topic, (int)payload_len, (const char *)payload);
}

static int on_connect(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
    (void)client;
    (void)arg;
    if (status != MQTT_CONNECT_ACCEPTED) {
        printf("Connection failed\n");
        return -1;
    }
    printf("Connected to MQTT broker\n");
    return 0;
}

static int on_disconnect(mqtt_client_t *client, void *arg)
{
    (void)client;
    (void)arg;
    printf("Disconnected from MQTT broker\n");
    return 0;
}

static int publish_state(mqtt_client_t *client, const char *state)
{
    mqtt_publish(client, 0, MQTT_TOPIC, state, strlen(state), MQTT_QOS_0, 0);
    printf("[Publish] State: %s\n", state);
    return 0;
}

void *mqtt_thread(void *arg)
{
    (void)arg;
    mqtt_client_t *client = &client;
    mqtt_client_init(client, inbuf, sizeof(inbuf), outbuf, sizeof(outbuf), on_pub, NULL);
    client->connect = on_connect;
    client->disconnect = on_disconnect;

    sock_udp_ep_t gw = { .family = AF_INET6, .port = MQTT_BROKER_PORT };
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, MQTT_BROKER_ADDRESS) == NULL) {
        printf("Unable to parse address\n");
        return NULL;
    }

    mqtt_connect(client, &gw, MQTT_KEEPALIVE_INTERVAL, MQTT_CLIENT_ID, NULL, NULL, 0, 0);

    while(1) {
        char state[16];
        snprintf(state, sizeof(state), "work");
        publish_state(client, state);
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void)
{
    puts("Starting MQTT example\n");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}