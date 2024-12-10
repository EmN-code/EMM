#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>

#define COAP_MAX_MSG_LEN 256
#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define POST_DATA "Hello, COAP Cloud"

void send_coap_post_request()
{
    int sock;
    struct sockaddr_in addr;
    struct coap_packet packet;
    uint8_t coap_buf[COAP_MAX_MSG_LEN];
    int r;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &addr.sin_addr);

    coap_packet_init(&packet, coap_buf, sizeof(coap_buf), 1, COAP_TYPE_CON, 0, NULL, 0, COAP_METHOD_POST, coap_next_id());
    coap_packet_append_option(&packet, COAP_OPTION_URI_PATH, URI_PATH, strlen(URI_PATH));
    coap_packet_append_payload_marker(&packet);
    coap_packet_append_payload(&packet, (uint8_t *)POST_DATA, strlen(POST_DATA));

    r = sendto(sock, packet.data, packet.offset, 0, (struct sockaddr *)&addr, sizeof(addr));
    if (r < 0) {
        printk("Failed to send CoAP packet\n");
    } else {
        printk("CoAP POST request sent successfully\n");
    }

    close(sock);
}

void main(void)
{
    printk("Starting CoAP client\n");
    send_coap_post_request();
}