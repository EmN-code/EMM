#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <stdio.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_client"
#define TOPIC "device/state"
#define MESSAGE "work"
#define STACK_SIZE 1024
#define THREAD_PRIORITY 5

// Allocate necessary buffers and structures
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static uint8_t payload[128];
struct mqtt_client client;
struct sockaddr_in broker_addr;

// Function for preparing the message
void prepare_mqtt_message(char *message) {
    snprintf(payload, sizeof(payload), "%s", message);
}

// Function to configure the MQTT client
void configure_mqtt_client(void) {
    mqtt_client_init(&client);

    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    net_addr_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    client.broker = &broker_addr;
    client.evt_cb = NULL;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENT_ID;
    client.client_id.size = strlen(MQTT_CLIENT_ID);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
}

// Thread for publishing MQTT messages
void publish_thread(void) {
    struct mqtt_publish_param param;

    // Initialize the MQTT parameters
    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)TOPIC;
    param.message.topic.topic.size = strlen(TOPIC);

    param.message.payload.data = payload;
    param.message.payload.len = sizeof(payload);
    param.message_id = 1;
    param.dup_flag = 0;
    param.retain_flag = 0;

    while (1) {
        prepare_mqtt_message(MESSAGE);

        mqtt_publish(&client, &param);

        k_sleep(K_SECONDS(5));
    }
}

// Main function
int main(void) {
    configure_mqtt_client();

    mqtt_connect(&client);

    // Start the MQTT publishing thread
    k_thread_create(&publish_thread, STACK_SIZE, THREAD_PRIORITY);

    return 0;
}