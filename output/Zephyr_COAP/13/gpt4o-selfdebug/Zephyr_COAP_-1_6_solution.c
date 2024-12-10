#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/sys/util.h>

#define COAP_BUF_SIZE 256
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

int main() {
    int sock;
    struct sockaddr_in server_addr;
    uint8_t coap_buf[COAP_BUF_SIZE];
    struct coap_packet request;
    ssize_t sent;

    /* Create a socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket: %d", sock);
        return -1;
    }

    /* Setup server address */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    /* Initialize CoAP request */
    coap_packet_init(&request, coap_buf, COAP_BUF_SIZE, 1, COAP_TYPE_CON, 8, coap_next_token(), COAP_METHOD_POST, coap_next_id());
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, (uint8_t *)URI_PATH, strlen(URI_PATH));

    /* Send CoAP request */
    sent = sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (sent < 0) {
        printk("Failed to send CoAP request: %d", sent);
    }
    else {
        printk("CoAP request sent successfully\n");
    }

    /* Close the socket */
    close(sock);

    return 0;
}