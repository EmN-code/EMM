#include <zephyr/net/coap.h>
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"
#define COAP_TOKEN 0x1234

int main() {
    struct sockaddr_in server_addr;
    struct coap_packet request;
    uint8_t buffer[128];
    int sock;

    /* Initialize server address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    /* Create socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Socket creation failed\n");
        return -1;
    }

    /* Initialize CoAP packet */
    coap_packet_init(&request, (uint8_t *)buffer, sizeof(buffer), COAP_TYPE_CON, COAP_TOKEN, COAP_METHOD_POST);

    /* Add URI path */
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, COAP_URI_PATH, strlen(COAP_URI_PATH));

    /* Add payload */
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)COAP_PAYLOAD, strlen(COAP_PAYLOAD));

    /* Send CoAP request */
    if (sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printk("CoAP request failed\n");
        close(sock);
        return -1;
    }

    printk("CoAP request sent\n");

    close(sock);
    return 0;
}