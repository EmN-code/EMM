#include <zephyr/kernel.h>
#include <net/mqtt.h>
#include <net/socket.h>

#define MQTT_BROKER_HOSTNAME "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENTID "esp32_client"
#define APP_SLEEP_MSECS 5000

static struct mqtt_client client;
static struct sockaddr_storage broker;

void prepare_fds(struct mqtt_client *client);
void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt);

void prepare_fds(struct mqtt_client *client) {
    /* Setting up file descriptors for select functionality */
    FD_ZERO(&client->fds);
    FD_SET(client->transport.tcp.sock, &client->fds);
}

void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt) {
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("MQTT client connected!");
        break;
    case MQTT_EVT_DISCONNECT:
        printk("MQTT client disconnected!");
        break;
    case MQTT_EVT_PUBLISH:
        printk("MQTT publish event received.");
        break;
    default:
        printk("Unhandled MQTT event %d", evt->type);
        break;
    }
}

static int publish(struct mqtt_client *client, enum mqtt_qos qos, char *data, size_t len) {
    struct mqtt_publish_param param;

    param.message.topic.qos = qos;
    param.message.topic.topic.utf8 = "sensors/esp32";
    param.message.topic.topic.size = strlen("sensors/esp32");
    
    param.message.payload.data = data;
    param.message.payload.len = len;
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    return mqtt_publish(client, &param);
}

void main(void) {
    int rc;
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    struct mqtt_client *client = &client;

    mqtt_client_init(client);

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_HOSTNAME, &broker4->sin_addr);

    client->broker = &broker;
    client->evt_cb = mqtt_evt_handler;

    client->client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
    client->client_id.size = strlen(MQTT_CLIENTID);

    mqtt_transport_connect(client, &broker);
    rc = mqtt_connect(client);

    if (rc != 0) {
        printk("Failed to connect to MQTT broker, error: %d
", rc);
        return;
    }

    char payload[] = "not work";

    while (1) {
        /* Change the payload to send "work" or "not work" alternatively */
        if (strcmp(payload, "work") == 0) {
            strcpy(payload, "not work");
        } else {
            strcpy(payload, "work");
        }

        rc = publish(client, MQTT_QOS_0_AT_MOST_ONCE, payload, strlen(payload));
        if (rc != 0) {
            printk("Failed to publish message, error: %d
", rc);
        }

        k_sleep(K_MSEC(APP_SLEEP_MSECS));
    }
}