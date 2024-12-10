
#include <zephyr.h>
#include <net/socket.h>
#include <net/coap.h>
#include <string.h>
#include <errno.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

void send_coap_request(int sock, const struct sockaddr *server_addr) {
    struct coap_packet request;
    struct coap_option options[1];
    uint8_t buffer[128];

    int ret = coap_packet_init(&request, buffer, sizeof(buffer), COAP_TYPE_CON, COAP_METHOD_POST, 0, NULL, 0, 0, 0);
    if (ret < 0) {
        printk("Failed to initialize CoAP packet: %d\n", ret);
        return;
    }

    ret = coap_packet_append_option(&request, COAP_OPTION_URI_PATH, URI_PATH, strlen(URI_PATH));
    if (ret < 0) {
        printk("Failed to append URI path option: %d\n", ret);
        return;
    }

    ret = coap_packet_append_payload_marker(&request);
    if (ret < 0) {
        printk("Failed to append payload marker: %d\n", ret);
        return;
    }

    ret = coap_packet_append_payload(&request, (const uint8_t *)PAYLOAD, strlen(PAYLOAD));
    if (ret < 0) {
        printk("Failed to append payload: %d\n", ret);
        return;
    }

    ret = sendto(sock, request.data, request.offset, 0, server_addr, sizeof(struct sockaddr_in));
    if (ret < 0) {
        printk("Failed to send CoAP request: %d\n", errno);
        return;
    }

    printk("CoAP request sent successfully\n");
}

int main(void) {
    int sock;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket: %d\n", errno);
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    send_coap_request(sock, (struct sockaddr *)&server_addr);

    close(sock);
    return 0;
}
