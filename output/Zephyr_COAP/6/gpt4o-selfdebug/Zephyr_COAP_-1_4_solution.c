#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>

#define COAP_BUF_SIZE 256

static const char *uri_path = "/coap/test";
static const char *coap_host = "47.102.103.1";
static const int coap_port = 5683;

void main(void) {
    uint8_t coap_buf[COAP_BUF_SIZE];
    struct coap_packet request;
    struct sockaddr_in server_addr;
    int sock;
    int ret;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to open socket
");
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(coap_port);
    inet_pton(AF_INET, coap_host, &server_addr.sin_addr);

    ret = coap_packet_init(&request, coap_buf, sizeof(coap_buf), COAP_VERSION_1,
                            COAP_TYPE_CON, COAP_SUPPORTED_TRANSACTIONS,
                            COAP_METHOD_POST, coap_next_id());
    if (ret < 0) {
        printf("Failed to initialize CoAP packet
");
        return;
    }

    coap_append_option(&request, COAP_OPTION_URI_PATH, uri_path, strlen(uri_path));

    const char *payload = "Hello, COAP Cloud";
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)payload, strlen(payload));

    ret = sendto(sock, coap_packet_get_data(&request), coap_packet_get_used(&request),
                 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        printf("Failed to send CoAP request
");
    }

    close(sock);
}
