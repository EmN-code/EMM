#include <zephyr.h>
#include <zephyr_device.h>
#include <random/rand32.h>
#include <sys/printk.h>
#include <net/socket.h>
#include <net/mqtt.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define PUB_TIME_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;

void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt) {
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("MQTT connected!");
        break;

    case MQTT_EVT_DISCONNECT:
        printk("MQTT disconnected!");
        break;

    default:
        break;
    }
}

static int publish_message(struct mqtt_client *client, char *topic, char *payload) {
    struct mqtt_publish_param param;
    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = topic;
    param.message.topic.topic.size = strlen(topic);
    param.message.payload.data = payload;
    param.message.payload.len = strlen(payload);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    return mqtt_publish(client, &param);
}

static void broker_init(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &broker4->sin_addr);
}

static int start_mqtt(void) {
    mqtt_client_init(&client);

    broker_init();

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)"zephyr_client";
    client.client_id.size = strlen("zephyr_client");

    struct mqtt_utf8 password = {"", 0};
    struct mqtt_utf8 user_name = {"", 0};

    client.user_name = &user_name;
    client.password = &password;

    return mqtt_connect(&client);
}

void main(void) {
    int rc;

    rc = start_mqtt();
    if (rc != 0) {
        printk("Failed to connect to broker (%d)", rc);
        return;
    }

    while (1) {
        rc = mqtt_live(&client);
        if (rc != 0) {
            printk("Error on mqtt_live: %d", rc);
        }

        rc = publish_message(&client, "state", "work or not work");
        if (rc != 0) {
            printk("Failed to publish message (%d)", rc);
        } else {
            printk("Message published!");
        }

        k_sleep(PUB_TIME_INTERVAL);
    }

    mqtt_disconnect(&client);
}