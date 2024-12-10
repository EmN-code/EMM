
#include <zephyr.h>
#include <net/socket.h>
#include <net/coap.h>
#include <logging/log.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

LOG_MODULE_REGISTER(coap_client);

void send_coap_request(int sock, const struct sockaddr *server_addr) {
    struct coap_packet request;
    struct coap_option options[1];
    uint8_t buffer[128];

    coap_packet_init(&request, buffer, sizeof(buffer), COAP_TYPE_CON, COAP_METHOD_POST, coap_next_token());
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, URI_PATH, strlen(URI_PATH));
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, PAYLOAD, strlen(PAYLOAD));

    if (sendto(sock, request.data, request.offset, 0, server_addr, sizeof(struct sockaddr_in)) < 0) {
        LOG_ERR("Failed to send CoAP request");
    } else {
        LOG_INF("CoAP request sent successfully");
    }
}

int main(void) {
    int sock;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    send_coap_request(sock, (struct sockaddr *)&server_addr);

    close(sock);
    return 0;
}
