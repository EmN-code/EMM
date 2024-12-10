#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)
#define MQTT_CLIENT_ID "esp32_zephyr_client"

/* MQTT client configuration */
static struct mqtt_client client;
static struct sockaddr_in broker;
static struct mqtt_publish_param pub_param;
static char mqtt_buffer[128];

static void prepare_mqtt_client(void) {
    mqtt_client_init(&client);
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);
    client.broker = &broker;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENT_ID;
    client.client_id.size = strlen(MQTT_CLIENT_ID);
    client.password = NULL;
    client.user_name = NULL;
    client.protocol_version = MQTT_VERSION_3_1_1;
}

void mqtt_event_handler(struct mqtt_client *const c, const struct mqtt_evt *evt) {
    /* Placeholder for handling incoming messages and errors */
}

static void publish_state(bool work_state) {
    const char *topic = "device/status";
    snprintf(mqtt_buffer, sizeof(mqtt_buffer), "State: %s", work_state ? "Work" : "Not Work");
    
    pub_param.message.payload.data = mqtt_buffer;
    pub_param.message.payload.len = strlen(mqtt_buffer);
    pub_param.message.topic.topic.utf8 = (uint8_t *)topic;
    pub_param.message.topic.topic.size = strlen(topic);
    pub_param.message_id = sys_rand32_get();
    pub_param.dup_flag = 0;
    pub_param.retain_flag = 0;
    
    mqtt_publish(&client, &pub_param);
}

void main(void) {
    struct mqtt_sec_config tls_config = { 0 };
    bool work_state = true;  // Alternate states for demonstration

    prepare_mqtt_client();
    mqtt_connect(&client);
    
    while (1) {
        publish_state(work_state);
        work_state = !work_state;
        k_sleep(PUBLISH_INTERVAL);
    }
}