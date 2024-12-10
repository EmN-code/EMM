#include <zephyr.h>
#include <net/coap.h>
#include <net/socket.h>
#include <sys/printk.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

int main() {
    struct sockaddr_in addr;
    struct coap_packet request;
    uint8_t request_data[128];
    int sock, ret;

    /* Create UDP socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    /* Initialize server address */
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &addr.sin_addr);

    /* Initialize CoAP packet */
    ret = coap_packet_init(&request, request_data, sizeof(request_data),
                           1, COAP_TYPE_CON, 0, NULL,
                           COAP_METHOD_POST, coap_next_id());
    if (ret < 0) {
        printk("Failed to initialize CoAP packet\n");
        close(sock);
        return -1;
    }

    /* Append URL path */
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH,
                              URI_PATH, strlen(URI_PATH));

    /* Append payload */
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, PAYLOAD, strlen(PAYLOAD));

    /* Send packet */
    ret = sendto(sock, request_data, request.offset, 0,
                 (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        printk("Failed to send CoAP request\n");
    } else {
        printk("CoAP request sent\n");
    }

    /* Close socket */
    close(sock);
    return 0;
}