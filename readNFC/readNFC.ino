#include <NfcAdapter.h>

#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>   // The following files are included in the libraries Installed
#include <NfcAdapter.h>

#include "SD.h"
#define SD_ChipSelectPin 53
#include "TMRpcm.h"
#include "SPI.h"
#define LED 7

TMRpcm tmrpcm;

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);  // Indicates the Shield you are using


void setup(void) {
  tmrpcm.speakerPin = 11;
  pinMode(LED , OUTPUT);
  Serial.begin(9600);
  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD fail");
    return;
  }
  
  Serial.begin(9600);
  Serial.println("NFC TAG READER"); // Header used when using the serial monitor
  nfc.begin();
}

void loop(void) {
  Serial.println("\nScan your NFC tag on the NFC Shield\n");  // Command so that you an others will know what to do 

  if (nfc.tagPresent())
  {
    NfcTag tag = nfc.read();
    Serial.println(tag.getTagType());
    Serial.print("UID: ");Serial.println(tag.getUidString()); // Retrieves the Unique Identification from your tag
    playWav(tag.getUidString());
    if (tag.hasNdefMessage()) // If your tag has a message
    {

      NdefMessage message = tag.getNdefMessage();
      Serial.print("\nThis Message in this Tag is ");
      Serial.print(message.getRecordCount());
      Serial.print(" NFC Tag Record");
      if (message.getRecordCount() != 1) { 
        Serial.print("s");
      }
      Serial.println(".");

      // If you have more than 1 Message then it wil cycle through them
      int recordCount = message.getRecordCount();
      for (int i = 0; i < recordCount; i++)
      {
        Serial.print("\nNDEF Record ");Serial.println(i+1);
        NdefRecord record = message.getRecord(i);

        int payloadLength = record.getPayloadLength();
        byte payload[payloadLength];
        record.getPayload(payload);


        String payloadAsString = ""; // Processes the message as a string vs as a HEX value
        for (int c = 0; c < payloadLength; c++) {
          payloadAsString += (char)payload[c];
        }
        Serial.print("  Information (as String): ");
        Serial.println(payloadAsString);
        Serial.print("My ID:");Serial.print(record.getId());

        String uid = record.getId();
        if (uid != "") {
          Serial.print("  ID: ");Serial.println(uid); // Prints the Unique Identification of the NFC Tag
          playWav(uid);
        }
      }
    }
  }
  if (tmrpcm.isPlaying()) {
    digitalWrite(LED , HIGH);//turn the LED On by making the voltage HIGH
    delay(500);
    digitalWrite(LED , LOW);// turn the LED Off by making the voltage LOW
  } else {
    digitalWrite(LED , LOW);// turn the LED Off by making the voltage LOW
  }
  delay(1000);
}

void playWav(String uid) {
  tmrpcm.setVolume(7);
  const char* uidStr = uid.c_str();
  Serial.print("Playing sound");
  //tmrpcm.play("sample1.wav");

  if (strcmp(uidStr, "80 C4 B1 DA") == 0) 
  {
    if (!tmrpcm.isPlaying()) {
      tmrpcm.play("sample1.wav");
    }
  } else if (strcmp(uidStr, "C0 14 B2 DA") == 0)
  {
    tmrpcm.play("sample4.wav");
  }
}
