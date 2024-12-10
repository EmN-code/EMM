#include <zephyr.h>
#include <sys/printk.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <string.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID   "zephyr_mqtt_client"
#define PUBLISH_TOPIC "state"
#define MSG "work or not work"
#define APP_SLEEP_MS 5000
#define APP_CONNECT_TRIES 10

static struct mqtt_client client;
static struct sockaddr_storage broker;
static bool connected = false;
static uint8_t tx_buffer[128];
static uint8_t rx_buffer[128];
static uint8_t payload[] = MSG;

void mqtt_event_handler(struct mqtt_client *const c, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result != 0) {
            printk("MQTT connect failed %d\n", evt->result);
            break;
        }
        connected = true;
        printk("MQTT client connected!\n");
        break;

    case MQTT_EVT_DISCONNECT:
        printk("MQTT client disconnected %d\n", evt->result);
        connected = false;
        break;

    case MQTT_EVT_PUBACK:
        printk("PUBACK packet id: %u\n", evt->param.puback.message_id);
        break;

    default:
        break;
    }
}

static int publish(struct mqtt_client *client)
{
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = PUBLISH_TOPIC;
    param.message.topic.topic.size = strlen(PUBLISH_TOPIC);
    param.message.payload.data = payload;
    param.message.payload.len = sizeof(payload);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0U;
    param.retain_flag = 0U;

    return mqtt_publish(client, &param);
}

static void broker_init(void)
{
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(SERVER_PORT);
    zsock_inet_pton(AF_INET, SERVER_ADDR, &broker4->sin_addr);
}

static void client_init(struct mqtt_client *client)
{
    mqtt_client_init(client);

    broker_init();

    client->broker = &broker;
    client->evt_cb = mqtt_event_handler;
    client->client_id.utf8 = (uint8_t *)CLIENT_ID;
    client->client_id.size = strlen(CLIENT_ID);
    client->password = NULL;
    client->user_name = NULL;
    client->protocol_version = MQTT_VERSION_3_1_1;

    client->rx_buf = rx_buffer;
    client->rx_buf_size = sizeof(rx_buffer);
    client->tx_buf = tx_buffer;
    client->tx_buf_size = sizeof(tx_buffer);
    client->transport.type = MQTT_TRANSPORT_NON_SECURE;
}

void main(void)
{
    int rc, i = 0;

    client_init(&client);

    while (i++ < APP_CONNECT_TRIES && !connected) {
        rc = mqtt_connect(&client);
        if (rc != 0) {
            printk("mqtt_connect failed %d\n", rc);
            k_sleep(K_SECONDS(1));
        }
        k_sleep(K_MSEC(APP_SLEEP_MS));
    }

    while (1) {
        rc = publish(&client);
        if (rc != 0) {
            printk("mqtt_publish failed %d\n", rc);
        } else {
            printk("Published to topic: %s\n", PUBLISH_TOPIC);
        }
        k_sleep(K_SECONDS(5));
    }
    mqtt_disconnect(&client);
}