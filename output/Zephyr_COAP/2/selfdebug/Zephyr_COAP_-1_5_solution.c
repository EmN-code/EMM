
#include <zephyr.h>
#include <net/socket.h>
#include <net/coap.h>
#include <logging/log.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

LOG_MODULE_REGISTER(coap_client);

void send_coap_request(int sock, const char *server_ip, uint16_t server_port, const char *uri_path, const char *message)
{
    struct sockaddr_in server_addr;
    struct coap_packet request;
    uint8_t buffer[128];
    int ret;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    ret = coap_packet_init(&request, buffer, sizeof(buffer), COAP_VERSION_1, COAP_TYPE_CON, 0, 0, 0, COAP_METHOD_POST, 0);
    if (ret < 0) {
        LOG_ERR("Failed to initialize CoAP packet (%d)", ret);
        return;
    }

    ret = coap_packet_append_option(&request, COAP_OPTION_URI_PATH, uri_path, strlen(uri_path));
    if (ret < 0) {
        LOG_ERR("Failed to append URI path option (%d)", ret);
        return;
    }

    ret = coap_packet_append_payload_marker(&request);
    if (ret < 0) {
        LOG_ERR("Failed to append payload marker (%d)", ret);
        return;
    }

    ret = coap_packet_append_payload(&request, message, strlen(message));
    if (ret < 0) {
        LOG_ERR("Failed to append payload (%d)", ret);
        return;
    }

    ret = sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        LOG_ERR("Failed to send CoAP request (%d)", ret);
        return;
    }

    LOG_INF("CoAP request sent successfully");
}

int main(void)
{
    int sock;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket (%d)", sock);
        return -1;
    }

    send_coap_request(sock, SERVER_IP, SERVER_PORT, URI_PATH, MESSAGE);

    close(sock);
    return 0;
}
