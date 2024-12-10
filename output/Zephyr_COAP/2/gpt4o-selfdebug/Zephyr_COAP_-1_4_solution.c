#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/net/socket.h>

#define COAP_BUF_SIZE 256
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"
#define COAP_PAYLOAD_LEN (sizeof(COAP_PAYLOAD) - 1)

void main(void) {
    struct coap_packet request;
    uint8_t buf[COAP_BUF_SIZE];
    uint8_t token[8] = {0};
    struct sockaddr_in server_addr;
    int sock;
    int r;

    printk("Starting CoAP client\n");

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket: %d\n", errno);
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(COAP_SERVER_PORT);
    inet_pton(AF_INET, COAP_SERVER_IP, &server_addr.sin_addr);

    r = coap_packet_init(&request, buf, COAP_BUF_SIZE,
                         1 /* Type CON */, COAP_METHOD_POST, 
                         token, sizeof(token), 
                         coap_next_id());
    if (r < 0) {
        printk("Failed to initialize CoAP message: %d\n", r);
        close(sock);
        return;
    }

    r = coap_packet_append_option(&request, COAP_OPTION_URI_PATH,
                                  COAP_URI_PATH, strlen(COAP_URI_PATH));
    if (r < 0) {
        printk("Failed to append URI_PATH: %d\n", r);
        close(sock);
        return;
    }

    r = coap_packet_append_payload_marker(&request);
    if (r < 0) {
        printk("Failed to append payload marker: %d\n", r);
        close(sock);
        return;
    }

    r = coap_packet_append_payload(&request, (uint8_t *)COAP_PAYLOAD, COAP_PAYLOAD_LEN);
    if (r < 0) {
        printk("Failed to append payload: %d\n", r);
        close(sock);
        return;
    }

    r = sendto(sock, request.data, request.offset, 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (r < 0) {
        printk("Failed to send CoAP request: %d\n", errno);
        close(sock);
        return;
    }

    printk("CoAP POST request sent\n");

    close(sock);
}
