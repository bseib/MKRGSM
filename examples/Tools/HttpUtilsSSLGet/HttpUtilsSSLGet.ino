/*
  This sketch Shows how to use the HTTP command to
  makes http request to the https end point and stores
  the result in the Sara u201 internal storage

  Circuit:
  - MKR GSM 1400 board

  Created 3 September 2020
  by Riccardo Rizzo

*/

#include <ArduinoMqttClient.h>
#include <MKRGSM.h>
#include "arduino_secrets.h"


const char PINNUMBER[]     = SECRET_PINNUMBER;
// APN data
const char GPRS_APN[]      = SECRET_GPRS_APN;
const char GPRS_LOGIN[]    = SECRET_GPRS_LOGIN;
const char GPRS_PASSWORD[] = SECRET_GPRS_PASSWORD;

// initialize the library instance
GSMHttpUtils httpClient;
GPRS gprs;
GSM gsmAccess;

GSMFileUtils fileUtils(false);

// URL, path and port (for example: example.org)
char server[] = "google.com";
char path[] = "/";
int port = 443; // port 80 is the default for HTTP

// An existing file
constexpr char* filename { "get.ffs" };


// Read bloack size
constexpr unsigned int blockSize { 512 };


const long interval = 1000;
unsigned long previousMillis = 0;

int count = 0;

void readFile();

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  bool connected = false;
  // After starting the modem with GSM.begin()
  // attach the shield to the GPRS network with the APN, login and password
  while (!connected) {
    if ((gsmAccess.begin(PINNUMBER) == GSM_READY) &&
        (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY)) {
      connected = true;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }
  fileUtils.begin(false);

  httpClient.enableSSL();
  httpClient.configServer(server, port);
  httpClient.gsmGet(path, filename);


}

void loop() {
  if (client.responseStatus()) {
    Serial.println("received");

    readFile();
    while (1);
  }
}

void readFile() {
  String fileList[10];
  auto size = fileUtils.listFiles(fileList);
  for (int i = 0; i < size && i <10; i++) {
    if (fileList[i] == filename) {
      Serial.print("file found: ");
      Serial.println(fileList[i]);
    }
    
  }
}
