#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_PORT 1883

/* Structure to represent MQTT client connection parameters */
static struct mqtt_client client;
static struct sockaddr_storage broker;

/* Function to set up client connection parameters */
static void prepare_mqtt_client(void)
{
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker4->sin_addr);

    client.broker = &broker;
    client.evt_cb = NULL;
    client.client_id.utf8 = (uint8_t *)"zephyr_pub";
    client.client_id.size = strlen("zephyr_pub");

    client.protocol_version = MQTT_VERSION_3_1_1;
}

/* Function to connect to the broker */
static int mqtt_connect_to_broker(void)
{
    prepare_mqtt_client();

    struct mqtt_sec_config tls_config = {0};
    return mqtt_connect(&client, &tls_config);
}

/* Function to publish data on the MQTT topic */
static int publish_state(bool working)
{
    struct mqtt_publish_param param;
    const char *topic = "device/status";
    const char *message = working ? "work" : "not work";

    param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)topic;
    param.message.topic.topic.size = strlen(topic);
    param.message.payload.data = (uint8_t *)message;
    param.message.payload.len = strlen(message);

    return mqtt_publish(&client, &param);
}

void main(void)
{
    int rc;

    rc = mqtt_connect_to_broker();
    if (rc != 0) {
        printk("Failed to connect to MQTT broker: %d\n", rc);
        return;
    }

    while (1) {
        publish_state(true);
        k_sleep(K_SECONDS(5));
    }
}