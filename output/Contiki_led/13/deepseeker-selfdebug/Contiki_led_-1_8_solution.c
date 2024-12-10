
#ifndef DHT11_SENSOR_H_
#define DHT11_SENSOR_H_

#include "sensors.h"

extern const struct sensors_sensor dht11_sensor;

#define DHT11_CONFIGURE_GPIO_PORT   (0)
#define DHT11_CONFIGURE_GPIO_PIN    (1)

#define DHT11_VALUE_HUMIDITY_INTEGER       (0)
#define DHT11_VALUE_HUMIDITY_DECIMAL       (1)
#define DHT11_VALUE_TEMPERATURE_INTEGER    (2)
#define DHT11_VALUE_TEMPERATURE_DECIMAL    (3)

#define DHT11_STATUS_OKAY               (0)
#define DHT11_STATUS_TIMEOUT            (1)
#define DHT11_STATUS_CHECKSUM_FAILED    (2)

#endif /* DHT11_SENSOR_H_ */
