#define SSID_COUNT;
#ifndef __SSID_CONFIG_H__
#define __SSID_CONFIG_H__

#define WIFI_SSID "TG1672GF2"
#define WIFI_PASS "TG1672GB7DDF2"


#endif // __SSID_CONFIG_H__

struct Creds{
	char* username;
	char* password;
};

void add_ssid(Creds *cred);