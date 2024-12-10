#include zephyr/net/socket_poll.h
#include zephyr/net/net_mgmt.h
#include zephyr/net/coap.h
#include zephyr/logging/log.h
#include zephyr/kernel.h
#include zephyr/device.h

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_VERSION 1
#define COAP_TYPE CON
#define COAP_TOKEN_LEN 0
#define COAP_TOKEN_NONE NULL

LOG_MODULE_REGISTER(coap_client_sample);

static int send_coap_request(void)
{
    struct coap_packet request_packet;
    uint8_t request_data[128];
    uint8_t token[COAP_TOKEN_LEN] = COAP_TOKEN_NONE;
    
    int ret = coap_packet_init(&request_packet, request_data, sizeof(request_data),
                               COAP_VERSION, COAP_TYPE_CON, COAP_TOKEN_LEN,
                               token, COAP_METHOD_POST, coap_header_get_id(&request_packet));
    if (ret < 0) {
        LOG_ERR("Failed to init CoAP packet: %d", ret);
        return ret;
    }

    const char* payload = "Hello, COAP Cloud";
    ret = coap_packet_append_payload_marker(&request_packet);
    if (ret < 0) {
        LOG_ERR("Failed to append payload marker: %d", ret);
        return ret;
    }
    
    ret = coap_packet_append_payload(&request_packet, (const uint8_t *)payload, strlen(payload));
    if (ret < 0) {
        LOG_ERR("Failed to append payload: %d", ret);
        return ret;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket: %d", errno);
        return -errno;
    }

    ret = sendto(sock, request_packet.data, coap_packet_get_used(&request_packet), 0,
                 (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        LOG_ERR("Failed to send data: %d", errno);
        close(sock);
        return -errno;
    }

    close(sock);
    LOG_INF("CoAP request sent successfully.");
    return 0;
}

void main(void)
{
    LOG_INF("Starting CoAP client example...");
    send_coap_request();
}