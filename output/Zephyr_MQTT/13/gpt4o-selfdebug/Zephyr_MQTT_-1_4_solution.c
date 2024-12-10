#include <zephyr.h>
#include <net/mqtt.h>
#include <random/rand32.h>
#include <stdio.h>

#define BROKER_ADDRESS "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_PERIOD K_SECONDS(5)
#define TOPIC "device/state"

static struct mqtt_client client;
static struct sockaddr_in broker;
static char message[] = "device state: work";

static void prepare_fds(struct mqtt_client *client)
{
   /* Implementation for setting up file descriptors */
}

static void broker_init(void)
{
    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDRESS, &broker.sin_addr);
}

static void mqtt_event_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
{
    /* Handle different MQTT events e.g., connection, messages, etc. */
}

void main(void)
{
    broker_init();

    struct mqtt_client client;
    mqtt_client_init(&client);

    struct mqtt_transport transport;
    transport.type = MQTT_TRANSPORT_TCP;
    client.transport = transport;

    client.broker = (struct sockaddr *)&broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = "zephyr_client";
    client.client_id.size = strlen(client.client_id.utf8);

    mqtt_connect(&client);

    while (1) {
        int rc = mqtt_publish(&client, &(struct mqtt_publish_param) {
            .message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE,
            .message.topic.topic.utf8 = TOPIC,
            .message.topic.topic.size = strlen(TOPIC),
            .message.payload.data = message,
            .message.payload.len = strlen(message),
            .message_id = sys_rand32_get()
        });

        if (rc) {
            printf("Unable to publish: %d", rc);
        }

        k_sleep(PUBLISH_PERIOD);
    }
}