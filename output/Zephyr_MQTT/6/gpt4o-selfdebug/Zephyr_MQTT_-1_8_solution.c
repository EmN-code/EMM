#include <zephyr/kernel.h>  // Include the Zephyr Kernel header for k_sleep, logging, etc.
#include <zephyr/logging/log.h>  // Zephyr logging for debugging purposes.
#include <zephyr/net/mqtt.h>  // Zephyr's MQTT library.
#include <zephyr/net/socket.h>  // For socket operations used in MQTT transport. 

#define BROKER_ADDR "47.102.103.1"  // Define the broker address
#define BROKER_PORT 1883  // Define the broker port
#define PUB_INTERVAL K_SECONDS(5)  // Publish interval of 5 seconds

LOG_MODULE_REGISTER(mqtt_pub, LOG_LEVEL_INF);  // Initialize logging module

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct mqtt_publish_param pub_param;

/* The MQTT client thread */
void mqtt_thread(void)
{
    int rc;
    struct mqtt_utf8 client_id = {.utf8 = "esp32_mqtt_client", .size = strlen("esp32_mqtt_client")};

    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = NULL;  // You could define a callback to handle events
    client.client_id = client_id;
    client.protocol_version = MQTT_VERSION_3_1_1;

    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(BROKER_PORT);
    zsock_inet_pton(AF_INET, BROKER_ADDR, &broker4->sin_addr);

    rc = mqtt_connect(&client);
    if (rc != 0) {
        LOG_ERR("MQTT connect failed %d", rc);
        return;
    }

    pub_param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    pub_param.message.topic.topic.utf8 = "state_topic";
    pub_param.message.topic.topic.size = strlen("state_topic");
    pub_param.message.payload.data = "work or not work";
    pub_param.message.payload.len = strlen("work or not work");
    pub_param.message_id = sys_rand32_get();
    pub_param.dup_flag = 0;
    pub_param.retain_flag = 0;

    while (1) {
        rc = mqtt_publish(&client, &pub_param);
        if (rc != 0) {
            LOG_ERR("MQTT publish failed %d", rc);
        }
        k_sleep(PUB_INTERVAL);
    }
}

void main(void)
{
    LOG_INF("MQTT loop start");
    mqtt_thread();
}