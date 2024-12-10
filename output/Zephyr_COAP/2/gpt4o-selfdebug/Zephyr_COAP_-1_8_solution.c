#include <zephyr.h>
#include <net/socket.h>
#include <net/coap.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_PATH "/coap/test"
#define BUF_SIZE 128

void main(void) {
    struct sockaddr_in addr;
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &addr.sin_addr);

    struct coap_packet request;
    uint8_t buffer[BUF_SIZE];

    coap_packet_init(&request, buffer, sizeof(buffer), COAP_TYPE_CON, COAP_METHOD_POST, coap_next_token(), coap_next_message_id());
    coap_packet_append_payload_marker(&request);
    const char *payload = "Hello, COAP Cloud";
    coap_packet_append_payload(&request, (const uint8_t *)payload, strlen(payload));
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, (const uint8_t *)COAP_PATH, strlen(COAP_PATH));

    int r = sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&addr, sizeof(addr));
    if (r < 0) {
        printk("Failed to send data\n");
    } else {
        printk("Sent request to CoAP server\n");
    }

    close(sock);
}