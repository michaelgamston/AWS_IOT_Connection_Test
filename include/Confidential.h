#include <Arduino.h>

#define SECRET
#define THINGNAME "TEST_LAMP_1"

<<<<<<< HEAD
const char WIFI_SSID[] = "****************";
const char WIFI_PASSWORD[] = "*****************";
const char AWS_IOT_ENDPOINT[] = "***************.iot.eu-west-2.amazonaws.com";
=======
const char WIFI_SSID[] = "************";
const char WIFI_PASSWORD[] = "*********";
const char AWS_IOT_ENDPOINT[] = "**************.iot.eu-west-2.amazonaws.com";
>>>>>>> 8382e9a5c83ed39c9216f518c517e109cadb98a0

// Amazon Root CA 1

static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";

// Device Certificate

static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)KEY";

// Device Private Key

static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----

-----END RSA PRIVATE KEY-----
)KEY";

extern std::string wifi_ssid;
extern std::string wifi_password;
