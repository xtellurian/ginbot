// (c) Copyright 2010-2012 MCQN Ltd.
// Released under Apache License, version 2.0
//
// Simple example to show how to use the HttpClient library
// Get's the web page given at http://<kHostname><kPath> and
// outputs the content to the serial port

#include <SPI.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

// This example downloads the URL "http://arduino.cc/"

#include "arduino_secrets.h"

#define AREF 5 // set to AREF, typically board voltage like 3.3 or 5.0
#define ADC_RESOLUTION 10  // set to ADC bit resolution, 10 is default
#define S1 A0
#define S2 A1

// analog input, its range may vary from 0 to 4095.

float  m, b;
float s1_reading, s1_voltage, s1_temperature;
float s2_reading, s2_voltage, s2_temperature;

float get_voltage(int raw_adc) {
  return raw_adc * (AREF / (pow(2, ADC_RESOLUTION) - 1));
}

// Temperature = (Vout - 1.25) / 0.005 V
float get_temperature(float voltage) {
  return (voltage - 1.25) / 0.005;
}

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// Wifi Settings ///////
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;


// Name of the server we want to connect to
const char kHostname[] = "192.168.182.204";
// Path to download (this is the bit after the hostname in the URL
// that you want to download
const char kPath[] = "/data";

// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30*1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;

WiFiClient c;
HttpClient http(c, kHostname, 5000);

void setup()
{
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // sensor pins
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // unsuccessful, retry in 4 seconds
    Serial.print("failed ... ");
    delay(4000);
    Serial.print("retrying ... ");
  }

  Serial.println("connected to WiFi");
}

void loop()
{

  // sensors
  s1_reading = analogRead(S1);
  s1_voltage = get_voltage(s1_reading);
  s1_temperature = get_temperature(s1_voltage);

  s2_reading = analogRead(S2);
  s2_voltage = get_voltage(s2_reading);
  s2_temperature = get_temperature(s2_voltage);
  
  int err =0;


  String contentType = "application/json";
  String postData = "{\"Value\":" + String(s1_temperature) + "}";
  Serial.println(postData);
  http.post(kPath, contentType, postData);
  
  if (err == 0)
  {
    Serial.println("startedRequest ok");

    err = http.responseStatusCode();
    if (err >= 0)
    {
      Serial.print("Got status code: ");
      Serial.println(err);

      int bodyLen = http.contentLength();
      Serial.print("Content length is: ");
      Serial.println(bodyLen);
      Serial.println();
      Serial.println("Body returned follows:");
    
      // Now we've got to the body, so we can print it out
      unsigned long timeoutStart = millis();
      char c;
      // Whilst we haven't timed out & haven't reached the end of the body
      while ( (http.connected() || http.available()) &&
             (!http.endOfBodyReached()) &&
             ((millis() - timeoutStart) < kNetworkTimeout) )
      {
          if (http.available())
          {
              c = http.read();
              // Print out this character
              Serial.print(c);
             
              // We read something, reset the timeout counter
              timeoutStart = millis();
          }
          else
          {
              // We haven't got any data, so let's pause to allow some to
              // arrive
              delay(kNetworkDelay);
          }
      }
    }
    else
    {    
      Serial.print("Getting response failed: ");
      Serial.println(err);
    }
  }
  else
  {
    Serial.print("Connect failed: ");
    Serial.println(err);
  }
  http.stop();
  Serial.println("...");
  Serial.println("gday");
  delay(1000);
}
