#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(udp_sample, LOG_LEVEL_DBG);

#include <zephyr/kernel.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/socket.h>


#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define PAYLOAD "Hello\n"

#define DEFAULT_NUM_SENDS 1000
#define DEFAULT_DELAY_US 100000

// wifi information
#define WIFI_SSID "EmNets-301"
#define WIFI_PSK "eagle402"

static int wifi_connected;
static struct net_mgmt_event_callback wifi_shell_mgmt_cb;

static void handle_wifi_connect_result(struct net_mgmt_event_callback *cb)
{
	const struct wifi_status *status = (const struct wifi_status *)
					   cb->info;

	if (status->status) {
		LOG_ERR("Connection request failed (%d)\n", status->status);
	} else {
		LOG_INF("WIFI Connected\n");
		wifi_connected = 1;
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

	wifi_connected = 0;

	LOG_INF("WIFI try connecting to %s...\n", WIFI_SSID);

	/* Let's wait few seconds to allow wifi device be on-line */
	while (nr_tries-- > 0) {
		ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &cnx_params,
			       sizeof(struct wifi_connect_req_params));
		if (ret == 0) {
			break;
		}

		LOG_ERR("Connect request failed %d. Waiting iface be up...\n", ret);
		k_msleep(500);
	}

	while (wifi_connected == 0) {
		k_msleep(100);
	}
}

void udp_send(void){
	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	int ret;
	if(sock<0){
		LOG_ERR("socket() failed: %d\n", errno);
		close(sock);
		return;
	}
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);
	ret = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(ret < 0){
		LOG_ERR("connect() failed: %d\n", errno);
		close(sock);
		return;
	}
	for(int i=0; i<DEFAULT_NUM_SENDS; i++){
		ret = sendto(sock, PAYLOAD, strlen(PAYLOAD), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
		if(ret < 0){
			LOG_ERR("sendto() failed: %d\n", errno);
			close(sock);
			return;
		}
		k_usleep(DEFAULT_DELAY_US);
	}

}

int main(void){
	
	wifi_connect();

	udp_send();
	
	return 0;
}