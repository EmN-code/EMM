#include <zephyr/net/coap.h>
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

static const struct sockaddr_in server_addr = {
    .sin_family = AF_INET,
    .sin_port = htons(COAP_SERVER_PORT),
    .sin_addr = { .s_addr = htonl(inet_addr(COAP_SERVER_IP)) }
};

static void send_coap_post(void) {
    int sock;
    struct coap_packet request;
    uint8_t pkt_buf[256];
    int r;

    /* Create CoAP socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket: %d\n", errno);
        return;
    }

    /* Initialize CoAP request packet */
    r = coap_packet_init(&request, pkt_buf, sizeof(pkt_buf),
                         COAP_VERSION_1, COAP_TYPE_CON,
                         0, NULL, COAP_METHOD_POST,
                         0);
    if (r < 0) {
        printk("Failed to init CoAP packet: %d\n", r);
        close(sock);
        return;
    }

    /* Set CoAP URI path */
    r = coap_packet_append_option(&request, COAP_OPTION_URI_PATH,
                                  COAP_URI_PATH, strlen(COAP_URI_PATH));
    if (r < 0) {
        printk("Failed to append URI_PATH option: %d\n", r);
        close(sock);
        return;
    }

    /* Set CoAP payload */
    r = coap_packet_append_payload_marker(&request);
    if (r < 0) {
        printk("Failed to append payload marker: %d\n", r);
        close(sock);
        return;
    }

    r = coap_packet_append_payload(&request, (uint8_t *)COAP_MESSAGE,
                                   strlen(COAP_MESSAGE));
    if (r < 0) {
        printk("Failed to append payload: %d\n", r);
        close(sock);
        return;
    }

    /* Send the CoAP request */
    r = sendto(sock, request.data, request.offset, 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (r < 0) {
        printk("Failed to send CoAP request: %d\n", errno);
        close(sock);
        return;
    }

    printk("CoAP POST request sent successfully\n");

    close(sock);
}

void main(void) {
    while (1) {
        send_coap_post();
        k_sleep(K_SECONDS(10));
    }
}
