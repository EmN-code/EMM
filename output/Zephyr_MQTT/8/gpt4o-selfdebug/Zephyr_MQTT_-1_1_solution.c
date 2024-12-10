#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>

#define BROKER_ADDR  "47.102.103.1"
#define BROKER_PORT  1883
#define PUB_TOPIC    "esp32/state"
#define PUB_MSG      "work or not work"
#define PUB_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_in broker;

void prepare_mqtt_client(void) {
    mqtt_client_init(&client);
    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);

    client.broker = &broker;
    client.evt_cb = NULL;
}

int publish_state(void) {
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = PUB_TOPIC;
    param.message.topic.topic.size = strlen(PUB_TOPIC);
    param.message.payload.data = PUB_MSG;
    param.message.payload.len = strlen(PUB_MSG);
    param.dup_flag = 0;
    param.retain_flag = 0;
    
    return mqtt_publish(&client, &param);
}

int main(void) {
    int rc;
    prepare_mqtt_client();

    rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("Failed to connect to MQTT broker: %d\n", rc);
        return rc;
    }

    while (1) {
        rc = publish_state();
        if (rc != 0) {
            printk("Failed to publish message: %d\n", rc);
        }
        k_sleep(PUB_INTERVAL);
    }

    return 0;
}