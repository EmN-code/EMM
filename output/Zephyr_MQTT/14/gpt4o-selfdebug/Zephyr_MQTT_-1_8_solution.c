#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_NOT "not work"

static struct mqtt_client client;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static struct sockaddr_storage broker;

static void mqtt_event_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("MQTT connected\n");
        break;
    case MQTT_EVT_DISCONNECT:
        printk("MQTT disconnected\n");
        break;
    default:
        break;
    }
}

static void broker_init(void)
{
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker4->sin_addr);
}

static void mqtt_publish(const char* message)
{
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.data = (uint8_t *)message;
    param.message.payload.len = strlen(message);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(&client, &param);
}

void main(void)
{
    int rc;

    broker_init();

    struct mqtt_utf8 client_id = {
        .utf8 = (uint8_t *)"zephyr_client",
        .size = strlen("zephyr_client")
    };

    struct mqtt_utf8 username = {
        .utf8 = NULL,
        .size = 0
    };

    struct mqtt_utf8 password = {
        .utf8 = NULL,
        .size = 0
    };

    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id = client_id;
    client.user_name = &username;
    client.password = &password;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.protocol_version = MQTT_VERSION_3_1_1;

    rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("Failed to connect to broker. Error: %d\n", rc);
        return;
    }

    while (true) {
        mqtt_input(&client);
        mqtt_live(&client);

        mqtt_publish(MQTT_PAYLOAD);
        k_sleep(K_SECONDS(5));
    }
}