/*
Universidad Nacional Autónoma de México
Facultad de Ciencias
Licenciatura en Ciencias de la Computación
Seminario de Ciencias de la Computación A: Introducción al Internet de las Cosas

Escrito por: M. en I. Valente Vázquez Velázquez
Última modificación: 23-marzo-2023

https://sites.google.com/ciencias.unam.mx/introduccion-iot-fc/inicio
*/



#include <pgmspace.h>
 
#define SECRET
#define THINGNAME "ESP32_al4zka"   //Nombre del dispositivo
 
const char WIFI_SSID[] = "Totalplay-3AAA";    //Nombre de red
const char WIFI_PASSWORD[] = "3AAAFF454wKvNXwP";   //Contraseña de red
const char AWS_IOT_ENDPOINT[] = "a1nkv4p2915pvs-ats.iot.us-east-2.amazonaws.com";   //Broker AWS


// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";
 
// Device Certificate                                               //change this
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUZZTDVD14LeX+ItwLWM2LqaFrOaAwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIzMDMyNjIyMzEw
OVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANmYF0H2nXCoGw6QYG4x
G4EasdZeyitPl8Khw2plzI5Ef23qpdTD1Fm2uao5ja94uSis5ghPtoHBwsdyr4xr
sBxaw5axrKakzZLPvK7VTA8y+rqBT5csbIUQFxgnBsnm194jVde9DBYJrOhQnCb/
xJTDjYHCBcyj+9JJokMXLnw/7/OIUTw5Z+QST27POZU8luodj3s+KaLUko+IQbog
BeTzFzkoqXzGrJ+Jzh5oOhfWws/ZWmIsf0eFgemZR+nwcU5YKgfw7quhk8nOhLQ7
IwGvGOcujEK/iIX+HVXYPppfDG4yDn1K8zPVojrBJOvbgzbYF/NREnGbwzYGOUI6
ZSsCAwEAAaNgMF4wHwYDVR0jBBgwFoAUheYKnJTbDxJcBg9PDakJnDdsYpYwHQYD
VR0OBBYEFIggF5nbUv7xNnF8dRoks7JzklTwMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQAweP1PLwIwgJpHR57B/d0sjdqT
hl2WRt2+wLrXlK9W9McJvo6FapG2EKIRND/0nAdS1q1GG7q4Q8hor1H7Tz3a3I4M
fxDf4BtHIxL0lWiLHpaVgiv7lbGNbxJqYQ+a/gtbYpz7zibCDskMXpeeCdGXpYCl
OUXaKJQzo2Pva8TzKy9iqyQBlp/kXPrIwRJx7G2bmj9g9aouCW1mukq49an/m21R
sNBCElghfgztaaa/BiB38w2JdfPAUYjIQdQR2TdLfsHMOMAOun0GNPShsu2RlYey
B3vp9AuC4GzBXDqD0ddIFRXF1ZbCRg1meFab5N3SQWcwY0rj+mPF8PxsN7Oc
-----END CERTIFICATE-----

)KEY";
// Device Private Key                                               //change this
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEA2ZgXQfadcKgbDpBgbjEbgRqx1l7KK0+XwqHDamXMjkR/beql
1MPUWba5qjmNr3i5KKzmCE+2gcHCx3KvjGuwHFrDlrGspqTNks+8rtVMDzL6uoFP
lyxshRAXGCcGyebX3iNV170MFgms6FCcJv/ElMONgcIFzKP70kmiQxcufD/v84hR
PDln5BJPbs85lTyW6h2Pez4potSSj4hBuiAF5PMXOSipfMasn4nOHmg6F9bCz9la
Yix/R4WB6ZlH6fBxTlgqB/Duq6GTyc6EtDsjAa8Y5y6MQr+Ihf4dVdg+ml8MbjIO
fUrzM9WiOsEk69uDNtgX81EScZvDNgY5QjplKwIDAQABAoIBACNqkOaIqGmMwxKk
xz1IOuzRln6I4SkZhF8Gi1MHDLtfRMp0NdeipC6SAjhFs90KrkCDm9FggBkwm25x
KUeWw4xp7Ynak3pwOpGZxGFEkGOs7YhrwbOeZG9oBgpFV+KG8z3RzIgX2h7AJmeN
r0zdtnZsOd0T4nPDgRpiJ8n5vPQ1+xgVtOTxEVJX4wwdZSmhXKyG86V1F6+8Pzof
ftlRH7QT2z9/VtLZDOjSZcunBLur7dtFCgcrqFpJEzj2aU05NSv4SrE5bj3R+KhN
AKQLJzJHQPCvq0tkJELIY33494+scTrCWQQ7cCu1lCeDWsyHmdpssok/RbUwbg2R
qYvnPbkCgYEA7bgm9XMlwA9Cj0X6q+1PQBbntiNaIdtfd934GqbhutNa3a7jHhw3
pjixa+yXwCMwYOelXdTOBp/IEnDu0xo4baNtoLDIdZmyAdtGKYDk4iHxU55aMgUJ
j6rnVJV+tugxfuEDKXbNOfoBlvGYXa0UehfjUK2ZH0pnkRyTvoEQGecCgYEA6lO+
jFIEdS7aCXQCYp/sOlcw8vDR+sBtYJ5YqsN4cWQEw/u53HExdS9yloMCHyA9nPC/
0rFHihqjXCzQIPd/UZCgOAro1vkdkK1iAcEU2YXaccrkDGj4lA+PTUQ9FAEWXzpl
WrQpqyKBzUsjlkP5yOwcz2Kqjy9oiEG5idxKGh0CgYEAwC+5j4bgZj3wGdUBrC3X
fIEIJADAxc6NiGM0SpfQtzJYrBDQHW/Cref3yMEmjnfkjPO6hfmd900Ge6JB8vmn
1TvjTZ6e8jSn7UhRmQxT/15htLC5Suc3q6cIDyItp8JhAar423Toxo6Q6m+4N0PO
4kadyIQQ33NP98mayNGfKtECgYEAoSMDdqTz8c1NHJENDtsgbCIHRBi8nK10yas6
F/uhOIBqEodpESE5vHbrWnsum07lb6XSk0Vbh6m3yqv6V6E6iVFmDAPch+/5F8NT
ERfAibDVPckitzH8NnzYs26IfdgdlVCn4A7iVI+/KPhRuYR3IWbUjPwxg5M7U++2
cgntXzUCgYAtAR4Rt7b2LiKBA1rmOxJORD6AbIaiA8Lz/ZtxdWzJqNCUpaFF7b4a
1EdPST8IL2ZfYF4yWYNAife/Bnlft3ffAGDU8lUmYMXflTJ/jkgFGOvvRT5Z+S/e
CSQ5KuS7cL9bUUmYmkVfSC+wDG+hXyC4h3vC6MFU5S3rTygEBLqrVA==
-----END RSA PRIVATE KEY-----
)KEY";
