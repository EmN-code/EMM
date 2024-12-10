#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <string.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)

static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static struct mqtt_client client;
static struct sockaddr_in broker;

static void mqtt_evt_handler(struct mqtt_client *const client, 
			 const struct mqtt_evt *evt) {
	// MQTT event handling logic can be implemented here.
}

static int init_mqtt_client(void) {
	mqtt_client_init(&client);

	broker.sin_family = AF_INET;
	broker.sin_port = htons(MQTT_BROKER_PORT);
	inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

	client.broker = &broker;
	client.evt_cb = mqtt_evt_handler;
	client.client_id.utf8 = (uint8_t *)"zephyr_client";
	client.client_id.size = strlen("zephyr_client");
	client.protocol_version = MQTT_VERSION_3_1_1;

	client.rx_buffer = rx_buffer;
	client.rx_buffer_size = sizeof(rx_buffer);
	client.tx_buffer = tx_buffer;
	client.tx_buffer_size = sizeof(tx_buffer);

	client.transport.type = MQTT_TRANSPORT_NON_SECURE;
	return mqtt_connect(&client);
}

void main(void) {
	int ret = init_mqtt_client();
	if (ret != 0) {
		// Handle connection error.
		return;
	}

	while (1) {
		struct mqtt_publish_param param;
		param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
		param.message.topic.topic.utf8 = (uint8_t *)"state";
		param.message.topic.topic.size = strlen("state");
		param.message.payload.data = (uint8_t *)"work";
		param.message.payload.len = strlen("work");
		param.message_id = 0;
		param.dup_flag = 0;
		param.retain_flag = 0;
		mqtt_publish(&client, &param);

		k_sleep(PUBLISH_INTERVAL);
	}
}