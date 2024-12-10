#include <zephyr/kernel.h>
#include <zephyr/mqtt/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/tls_credentials.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct mqtt_publish_param param;
static char payload[] = "work or not work";

void prepare_mqtt_client(struct mqtt_client *client)
{
    /* MQTT client configuration */
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    net_addr_pton(AF_INET, MQTT_BROKER_ADDR, &broker4->sin_addr);

    mqtt_client_init(client);

    client->broker = &broker;
    client->evt_cb = NULL;
    client->client_id.utf8 = (uint8_t *)"zephyr_pub";
    client->client_id.size = strlen("zephyr_pub");
    client->password = NULL;
    client->user_name = NULL;
    client->protocol_version = MQTT_VERSION_3_1_1;
    client->rx_buf = NULL;
    client->rx_buf_size = 0;
    client->tx_buf = NULL;
    client->tx_buf_size = 0;
}

void publish_state(struct mqtt_client *client)
{
    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)"state/topic";
    param.message.topic.topic.size = strlen("state/topic");
    param.message.payload.data = payload;
    param.message.payload.len = sizeof(payload);
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(client, &param);
}

int main(void)
{
    int err;
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    prepare_mqtt_client(&client);

    err = mqtt_connect(&client);
    if (err != 0) {
        printk("Failed to connect to MQTT broker: %d\n", err);
        return 1;
    }

    while (1) {
        publish_state(&client);
        k_sleep(PUBLISH_INTERVAL);
    }

    mqtt_disconnect(&client);

    return 0;
}