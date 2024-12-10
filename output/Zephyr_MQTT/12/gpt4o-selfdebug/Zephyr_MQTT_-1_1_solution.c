#include <zephyr/net/socket.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
#include <zephyr/net/mqtt.h>

LOG_MODULE_REGISTER(mqtt_publisher, LOG_LEVEL_DBG);

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENTID "zephyr_mqtt_client"
#define MQTT_TOPIC "device/state"
#define MQTT_MSG "work or not work"

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct mqtt_publish_param pub_param;

/* Function to initialize the MQTT client */
static void mqtt_client_init(void)
{
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    mqtt_client_init(&client);

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker4->sin_addr);

    client.broker = &broker;
    client.evt_cb = NULL;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
    client.client_id.size = strlen(MQTT_CLIENTID);
    client.password = NULL;
    client.user_name = NULL;

    pub_param.message.payload.data = MQTT_MSG;
    pub_param.message.payload.len = strlen(MQTT_MSG);
    pub_param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    pub_param.message_id = sys_rand32_get();
    pub_param.topic.topic.utf8 = (uint8_t *)MQTT_TOPIC;
    pub_param.topic.topic.size = strlen(MQTT_TOPIC);
}

void main(void)
{
    int rc;

    mqtt_client_init();

    printk("Connecting to MQTT broker...");

    rc = mqtt_connect(&client);
    if (rc != 0) {
        LOG_ERR("Could not connect to MQTT broker: %d", rc);
        return;
    }

    while (1) {
        mqtt_input(&client);
        mqtt_live(&client);

        rc = mqtt_publish(&client, &pub_param);
        if (rc != 0) {
            LOG_ERR("Could not publish message: %d", rc);
        } else {
            printk("Message published.");
        }

        k_sleep(K_SECONDS(5));
    }

    mqtt_disconnect(&client);
}