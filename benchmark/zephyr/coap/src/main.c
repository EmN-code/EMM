#include <zephyr/kernel.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/socket.h>

// wifi information
#define WIFI_SSID "EmNets-301"
#define WIFI_PSK "eagle402"

// coap server information
#define COAP_SERVER_ADDR "10.214.131.232"
#define COAP_SERVER_PORT 14023
#define COAP_RESOURCE_PATH "basic"
#define PAYLOAD "hello, coap server"
#define MAX_COAP_MSG_LEN 256

static int connected;
static struct net_mgmt_event_callback wifi_shell_mgmt_cb;

static int sock;

static void handle_wifi_connect_result(struct net_mgmt_event_callback *cb)
{
	const struct wifi_status *status = (const struct wifi_status *)
					   cb->info;

	if (status->status) {
		printk("Connection request failed (%d)\n", status->status);
	} else {
		printk("WIFI Connected\n");
		connected = 1;
	}
}

static void wifi_mgmt_event_handler(struct net_mgmt_event_callback *cb,
				    uint32_t mgmt_event, struct net_if *iface)
{
	switch (mgmt_event) {
	case NET_EVENT_WIFI_CONNECT_RESULT:
		handle_wifi_connect_result(cb);
		break;
	default:
		break;
	}
}

void wifi_connect(void)
{
	int nr_tries = 10;
	int ret = 0;

	net_mgmt_init_event_callback(&wifi_shell_mgmt_cb,
				     wifi_mgmt_event_handler,
				     NET_EVENT_WIFI_CONNECT_RESULT);

	net_mgmt_add_event_callback(&wifi_shell_mgmt_cb);

	struct net_if *iface = net_if_get_default();
	static struct wifi_connect_req_params cnx_params = {
		.ssid = WIFI_SSID,
		.ssid_length = strlen(WIFI_SSID),
		.psk = WIFI_PSK,
		.psk_length = strlen(WIFI_PSK),
		.channel = WIFI_CHANNEL_ANY,
		.security = WIFI_SECURITY_TYPE_PSK,
		.band = WIFI_FREQ_BAND_2_4_GHZ,
	};

	connected = 0;

	printk("WIFI try connecting to %s...\n", WIFI_SSID);

	/* Let's wait few seconds to allow wifi device be on-line */
	while (nr_tries-- > 0) {
		ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &cnx_params,
			       sizeof(struct wifi_connect_req_params));
		if (ret == 0) {
			break;
		}

		printk("Connect request failed %d. Waiting iface be up...\n", ret);
		k_msleep(500);
	}

	while (connected == 0) {
		k_msleep(100);
	}
}

void send_coap_message(void){
	// post/put a coap message to the server
	int ret = 0; // record the return value of the function
	struct sockaddr_in server_addr; // server ipv4 address

	server_addr.sin_family = AF_INET; // ipv4
	server_addr.sin_port = htons(COAP_SERVER_PORT); // port
	inet_pton(AF_INET, COAP_SERVER_ADDR, &server_addr.sin_addr); // set server address

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // create a socket
	if (sock < 0) {
		printk("socket() failed: %d\n", errno);
		close(sock);
		return;
	}

	ret = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)); // connect to udp remote
	if (ret < 0) {
		printk("connect() failed: %d\n", errno);
		close(sock);
		return;
	}

	// init a coap packet
	uint8_t payload[] = PAYLOAD;
	struct coap_packet request;
	// const char * const *p;
	uint8_t *data;
	int r;

	data = (uint8_t *)k_malloc(MAX_COAP_MSG_LEN);
	if (!data) {
		printk("k_malloc() failed: %d\n", errno);
		close(sock);
		k_free(data);
		return;
	}

	r = coap_packet_init(&request, data, MAX_COAP_MSG_LEN, COAP_VERSION_1, COAP_TYPE_CON, COAP_TOKEN_MAX_LEN, coap_next_token(), COAP_METHOD_POST, coap_next_id());
	if(r < 0){
		printk("coap_packet_init() failed: %d\n", r);
		close(sock);
		k_free(data);
		return;
	}

	r = coap_packet_append_option(&request, COAP_OPTION_URI_PATH, COAP_RESOURCE_PATH, strlen(COAP_RESOURCE_PATH));
	if(r < 0){
		printk("coap_packet_append_option() failed: %d\n", r);
		close(sock);
		k_free(data);
		return;
	}

	r = coap_packet_append_payload_marker(&request);
	if(r < 0){
		printk("coap_packet_append_payload_marker() failed: %d\n", r);
		close(sock);
		k_free(data);
		return;
	}

	r = coap_packet_append_payload(&request, (uint8_t *)payload, sizeof(payload) - 1);
	if(r < 0){
		printk("coap_packet_append_payload() failed: %d\n", r);
		close(sock);
		k_free(data);
		return;
	}
	
	// send coap message
	r = send(sock, request.data, request.offset, 0);
	k_free(data);
	if(r < 0){
		printk("send() failed: %d\n", errno);
		close(sock);
		return;
	}else{
		printk("message sended\n");
		close(sock);
		return;
	}

}

int main(void){
	
	wifi_connect();
	while(1){
		send_coap_message();
		k_sleep(K_SECONDS(3));
	}
	
	return 0;
}