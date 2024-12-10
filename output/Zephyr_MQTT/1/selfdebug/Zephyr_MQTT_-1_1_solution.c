#include <zephyr/sys/printk.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/logging/log.h>

#define MQTT_CLIENTID        "zephyr_pub"
#define MQTT_BROKER_ADDR     "47.102.103.1"
#define MQTT_BROKER_PORT     1883
#define PUBLISH_INTERVAL     K_SECONDS(5)

LOG_MODULE_REGISTER(mqtt_publisher, LOG_LEVEL_INF);

static struct mqtt_client mqttClient;
static struct sockaddr_in broker;

static void configure_broker(void) {
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);
}

static int setup_mqtt_client(void) {
    struct mqtt_sec_config sec_config = {0};
    struct mqtt_transport transport;

    transport.type = MQTT_TRANSPORT_NON_SECURE;

    struct mqtt_client *client = &mqttClient;

    mqtt_client_init(client);

    client->broker = (struct sockaddr *)&broker;
    client->evt_cb = NULL;
    client->client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
    client->client_id.size = strlen(MQTT_CLIENTID);
    client->protocol_version = MQTT_VERSION_3_1_1;
    client->transport = &transport;

    return mqtt_connect(client);
}

static void mqtt_publish_handler(int state) {
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)"sensors/state";
    param.message.topic.topic.size = strlen("sensors/state");
    param.message.payload.data = (uint8_t *)(state ? "not work" : "work");
    param.message.payload.len = strlen(state ? "not work" : "work");
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(&mqttClient, &param);
}

void main(void) {
    int rc;
    int state = 0;

    LOG_INF("MQTT Publisher sample started");

    configure_broker();
    rc = setup_mqtt_client();
    if (rc != 0) {
        LOG_ERR("Failed to set up MQTT client: %d", rc);
        return;
    }

    while (1) {
        mqtt_publish_handler(state);
        state = !state;
        k_sleep(PUBLISH_INTERVAL);
    }
}