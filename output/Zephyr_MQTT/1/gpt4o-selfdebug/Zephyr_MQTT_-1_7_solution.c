#include <zephyr/zephyr.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/drivers/gpio.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883

#define MQTT_CLIENTID "zephyr_mqtt_client"

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

#define STACK_SIZE 1024
#define THREAD_PRIORITY 7

void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt) {
	// Handle MQTT events here
}

void mqtt_publish_state(struct mqtt_client *client) {
	struct mqtt_publish_param param;

	param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
	param.message.topic.topic.utf8 = "state";
	param.message.topic.topic.size = strlen("state");
	param.message.payload.data = "work"; // Can toggle between "work" and "not work"
	param.message.payload.len = strlen("work");
	param.message_id = sys_rand32_get();
	param.dup_flag = 0;
	param.retain_flag = 0;

	mqtt_publish(client, &param);
}

void mqtt_setup(void) {
	struct mqtt_sec_config *tls_config = NULL;
	mqtt_client_init(&client);

	broker4->sin_family = AF_INET;
	broker4->sin_port = htons(BROKER_PORT);
	inet_pton(AF_INET, BROKER_ADDR, &broker4->sin_addr);

	client.broker = &broker;
	client.evt_cb = mqtt_evt_handler;
	client.client_id.utf8 = MQTT_CLIENTID;
	client.client_id.size = strlen(MQTT_CLIENTID);
	client.protocol_version = MQTT_VERSION_3_1_1;
	client.transport.type = MQTT_TRANSPORT_NON_SECURE;
	client.rx_buf = malloc(STACK_SIZE);
	client.rx_buf_size = STACK_SIZE;
	client.tx_buf = malloc(STACK_SIZE);
	client.tx_buf_size = STACK_SIZE;

	mqtt_connect(&client);
}

void main(void) {
	mqtt_setup();

	while (1) {
		mqtt_publish_state(&client);
		k_sleep(K_SECONDS(5));
	}
}