#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENTID "zephyr_mqtt_example"
#define MQTT_TOPIC "device/state"
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct zsock_pollfd fds[1];
static bool connected;

static void prepare_mqtt_message(struct mqtt_publish_param *param, const char *msg)
{
    param->message.payload.data = msg;
    param->message.payload.len = strlen(msg);
    param->message.topic.topic.string = MQTT_TOPIC;
    param->message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
}

void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result == 0) {
                printk("Connected to MQTT broker\n");
                connected = true;
            }
            break;
        case MQTT_EVT_DISCONNECT:
            printk("Disconnected from MQTT broker\n");
            connected = false;
            break;
        // Add more MQTT event handling as needed
        default:
            break;
    }
}

int main(void)
{
    int rc;
    struct mqtt_publish_param param;

    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    zsock_inet_pton(AF_INET, MQTT_BROKER_IP, &broker4->sin_addr);

    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
    client.client_id.size = strlen(MQTT_CLIENTID);

    rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("Failed to connect to broker, error: %d\n", rc);
        return 1;
    }

    fds[0].fd = client.transport.tcp.sock;
    fds[0].events = ZSOCK_POLLIN;

    while (1) {
        if (connected) {
            mqtt_input(&client);
            mqtt_live(&client);

            prepare_mqtt_message(&param, "work");
            rc = mqtt_publish(&client, &param);
            if (rc != 0) {
                printk("Failed to publish, error: %d\n", rc);
            }

            k_sleep(PUBLISH_INTERVAL);
        } else {
            k_sleep(K_SECONDS(1));
        }
    }

    mqtt_disconnect(&client);
    return 0;
}