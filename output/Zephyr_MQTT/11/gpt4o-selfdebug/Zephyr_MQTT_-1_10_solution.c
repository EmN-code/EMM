#include <net/mqtt.h>  // For MQTT functions
#include <zephyr.h>     // Main Zephyr project API

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883

#define DEVICE_ID "esp32_mqtt_client"
#define TOPIC "device/state"
#define MESSAGE "state: work or not work"

#define APP_CONNECT_TRIES 10
#define PUB_PERIOD K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;

/* Buffer for MQTT client. */
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static uint8_t payload_buf[128];

static void prepare_fds(struct mqtt_client *client) {
	int nfds = 2; 
	struct pollfd fds[2] = {
		{.fd = client->transport.tcp.sock, .events = POLLIN},
		{.fd = client->transport.tcp.sock, .events = POLLOUT},
	};
}

static int publish(struct mqtt_client *client, enum mqtt_qos qos, char *data, size_t len) {
	struct mqtt_publish_param param;
	
	param.message.topic.qos = qos;
	param.message.topic.topic.utf8 = TOPIC;
	param.message.topic.topic.size = strlen(TOPIC);
	param.message.payload.data = data;
	param.message.payload.len = len;
	param.message_id = sys_rand32_get();
	param.dup_flag = 0U;
	param.retain_flag = 0U;

	return mqtt_publish(client, &param);
}

static void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt) {
	/* Placeholder event handler. */
}

void main(void) {
	struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
	
	mqtt_client_init(&client);
	
	broker4->sin_family = AF_INET;
	broker4->sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_ADDR, &broker4->sin_addr);

	client.broker = &broker;
	client.evt_cb = mqtt_evt_handler;
	client.client_id.utf8 = DEVICE_ID;
	client.client_id.size = strlen(DEVICE_ID);
	client.protocol_version = MQTT_VERSION_3_1_1;
	client.rx_buf = rx_buffer;
	client.rx_buf_size = sizeof(rx_buffer);
	client.tx_buf = tx_buffer;
	client.tx_buf_size = sizeof(tx_buffer);

	int rc = mqtt_connect(&client);

	if (rc != 0) {
		printf("mqtt_connect failed: %d
", rc);
		return;
	}

	/* Implement retry logic or exit logic here per project requirement. */
	
	while (1) {
		publish(&client, MQTT_QOS_1_AT_LEAST_ONCE, MESSAGE, strlen(MESSAGE));
		k_sleep(PUB_PERIOD);
	}

	mqtt_disconnect(&client);
}