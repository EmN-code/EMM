#include <zephyr/types.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/socket.h>
#include <zephyr/crypto/crypto.h>
#include <zephyr/kernel.h>
#include <zephyr/random/random.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>

/* Define CoAP server address and port */
#define SERVER_PORT 5683
#define SERVER_ADDR "47.102.103.1"
#define URI_PATH "/coap/test"

/* Define CoAP version, type, method */
#define COAP_VERSION 1
#define COAP_TYPE_CON 0
#define COAP_METHOD_POST 2

#define STACKSIZE 1024
#define PRIORITY 7

void main(void)
{
    int sock, r;
    struct sockaddr_in addr;
    uint8_t request_buf[64];
    struct coap_packet request;

    /* Initialize CoAP packet */
    coap_packet_init(&request, request_buf, sizeof(request_buf),
                     COAP_VERSION, COAP_TYPE_CON, 0,
                     NULL, COAP_METHOD_POST, coap_next_id());

    /* Append URI path to request */
    coap_packet_append_uri_path(&request, URI_PATH);

    /* Append payload */
    const char *payload = "Hello, COAP Cloud";
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)payload, strlen(payload));

    /* Create socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    /* Configure server address */
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &addr.sin_addr);

    /* Send CoAP request to server */
    r = sendto(sock, request.data, request.offset, 0,
               (struct sockaddr *)&addr, sizeof(addr));
    if (r < 0) {
        printk("Cannot send CoAP request: %d\n", errno);
    } else {
        printk("CoAP request sent successfully\n");
    }

    /* Close socket */
    close(sock);
}
