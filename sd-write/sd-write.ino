#include <SD.h>

File myFile;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Init...\n");

  pinMode(10, OUTPUT);

  if (!SD.begin(10)) {
    Serial.println("Init failed\n");
  }

  myFile = SD.open("test.txt", FILE_WRITE);

  if (myFile) {
    myFile.println("Some test data\n");
    myFile.close();
    Serial.println("done\n");
  } else {
    Serial.println("error opening file\n");
  }
  

}

void loop() {
  // put your main code here, to run repeatedly:

}
