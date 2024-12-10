#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define MQTT_TOPIC "state"
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;

static struct mqtt_pub_param pub_param;

static int publish_state(struct mqtt_client *client, const char *state) {
    pub_param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    pub_param.message.topic.topic.utf8 = MQTT_TOPIC;
    pub_param.message.topic.topic.size = strlen(MQTT_TOPIC);
    pub_param.message.payload.data = (uint8_t *)state;
    pub_param.message.payload.len = strlen(state);
    pub_param.message_id = k_cycle_get_32();

    return mqtt_publish(client, &pub_param);
}

static void mqtt_event_handler(struct mqtt_client *const client,
			 const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result != 0) {
                printk("MQTT connect failed: %d
", evt->result);
            }
            break;

        case MQTT_EVT_DISCONNECT:
            printk("MQTT disconnected
");
            break;

        default:
            break;
    }
}

static void configure_mqtt(struct mqtt_client *client) {
    mqtt_client_init(client);

    broker.ss_family = AF_INET;
    net_sin(&broker)->sin_addr.s_addr = inet_addr(SERVER_ADDR);
    net_sin(&broker)->sin_port = htons(SERVER_PORT);

    client->broker = &broker;
    client->evt_cb = mqtt_event_handler;
    client->client_id.utf8 = "zephyr_client";
    client->client_id.size = strlen(client_id.utf8);

    client->protocol_version = MQTT_VERSION_3_1_1;
}

void main(void) {
    int rc;
    printk("Starting MQTT publisher example
");

    configure_mqtt(&client);

    rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("Could not connect to MQTT broker: %d
", rc);
        return;
    }

    while (1) {
        rc = mqtt_input(&client);
        if (rc < 0) {
            printk("MQTT recv error: %d
", rc);
            mqtt_disconnect(&client);
            return;
        }

        rc = mqtt_live(&client);
        if (rc < 0) {
            printk("MQTT keep alive error: %d
", rc);
            mqtt_disconnect(&client);
            return;
        }

        rc = publish_state(&client, "work");
        if (rc < 0) {
            printk("Failed to publish state: %d
", rc);
        } else {
            printk("Published 'work' state
");
        }

        k_sleep(PUBLISH_INTERVAL);
    }
}