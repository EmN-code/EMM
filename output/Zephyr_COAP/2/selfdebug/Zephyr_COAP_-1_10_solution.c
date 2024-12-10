
#include <zephyr.h>
#include <net/socket.h>
#include <net/coap.h>
#include <logging/log.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

LOG_MODULE_REGISTER(coap_client);

void send_coap_request(int sock) {
    struct sockaddr_in server_addr;
    struct coap_packet request;
    uint8_t buffer[128];

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (coap_packet_init(&request, buffer, sizeof(buffer), COAP_VERSION_1, COAP_TYPE_CON, 4, NULL, COAP_METHOD_POST, coap_next_id()) < 0) {
        LOG_ERR("Failed to initialize CoAP packet");
        return;
    }

    if (coap_packet_append_option(&request, COAP_OPTION_URI_PATH, URI_PATH, strlen(URI_PATH)) < 0) {
        LOG_ERR("Failed to append URI path option");
        return;
    }

    if (coap_packet_append_payload_marker(&request) < 0) {
        LOG_ERR("Failed to append payload marker");
        return;
    }

    if (coap_packet_append_payload(&request, (const uint8_t *)PAYLOAD, strlen(PAYLOAD)) < 0) {
        LOG_ERR("Failed to append payload");
        return;
    }

    if (sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        LOG_ERR("Failed to send CoAP request");
        return;
    }

    LOG_INF("CoAP request sent successfully");
}

int main(void) {
    int sock;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket");
        return -1;
    }

    send_coap_request(sock);

    close(sock);
    return 0;
}
