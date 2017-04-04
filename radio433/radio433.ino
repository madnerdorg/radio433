//radio433.ino
// By Rémi Sarrailh
// MIT licence
// version: 1.2

// Get this program / update at: https://github.com/madnerdorg/radio433
//
// Commands are sent with a REST syntax (ex: /radio/text/Hello World)
// Data are received in JSON (ex: "{"data" : "/radio/text/Hello World"}")
// Protocols managed: RemoteTransmitter (old)/NewRemoteTransmitter (new)/RadioHead (text)

// /radio/new/1234/1/on : turn on new radio "address:1234 unit:1"
// /radio/new/1234/0/off : turn off new radio "address:1234 unit:0"
// /radio/new/1234/2/8 : dim new radio to 8/16 "address:1234 unit:2"

// /radio/old/123456 : send old radio code "123456"

// /radio/text/Hello world : send RadioHead code "Hello world"

// Library
// RadioHead by Airspayce.com : http://www.airspayce.com/mikem/arduino/RadioHead/
// 433mhzforarduino by Fuzzilogic : https://bitbucket.org/fuzzillogic/433mhzforarduino/

//PinOut
//ASK 433Mhz Transmitter                : 10
//ASK 433Mhz Receiver (RXB6 recommanded): 2

//Radio
//433MhzforArduino Libraries (Switchs)
#include <NewRemoteTransmitter.h>
#include <RemoteTransmitter.h>
#include <RemoteReceiver.h>
#include <NewRemoteReceiver.h>
#include <InterruptChain.h>

//RadioHead Libraries (Text message)
#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile

/*

    Setup
*/

//Version
const float VER = 1.2;

//Led
const int statusLedPin = 13;
const int NbledBlink = 20; //Number of times led blink

const String usb_name = "radio433:42002";

//Radio
const int txPin = 10; //Transmitter (You can change where it is plugged)
const int rxPin = 2; //Receiver (We need to use Interrupt 0 ==> Pin 2)

const int REPEATS = 2; //Repeats for /radio/new and /radio/old
const int BITRATE_RADIOHEAD = 2000; //RadioHead bitrate

boolean txStatus = false;
boolean rxStatus = false;

//Radio Head
RH_ASK driver(BITRATE_RADIOHEAD, rxPin, txPin, statusLedPin, false);
//RadioHead add latency which alter periods on 433mhzforarduino
//We compensate it by reducing periods when we send a text message
const int RADIOHEAD_LATENCY_CORRECTION = 50;

//Setup led/serial/radio
void setup() {
  //Setup status LED
  pinMode(statusLedPin, OUTPUT);

  //Serial Setup
  Serial.begin(115200);
  Serial.setTimeout(100);

  setupRadio(); // Setup Radio
  //sendInfo(); //Display status for this program
}

//Wait for serial input
void loop() {
  checkCommand();
}

/*


  Serial
*/

//Check if a command sent on serial is valid and sent it with radio
void checkCommand() {

  //If a message is sent in serial
  if (Serial.available() > 0)
  {
    //We assume command will be end by an new string (\n)
    String line = Serial.readStringUntil('\n');
    //We explode the string and get first string inside  /string/.../..../...
    String command = getValue(line, '/', 1);

    //If no command is supplied display help
    if (command == "") {
      Serial.println("{\"err\":\"INVALID COMMAND - /info,/radio\"}");
    } else {
      //If /radio/.../.../....
      if (command == "radio") {
        String radioType = getValue(line, '/', 2);
        if (radioType == "old") {
          sendOld434(line); //code switch
        }
        else if (radioType == "new") {
          sendNew434(line); //self learning switch
        }
        else if (radioType == "text") {
          sendText434(line); //radioHead message
        } else {
          //If no valid protocol found display help
          Serial.println("{\"err\":\"INVALID PROTOCOL - /radio/old,/radio/new,/radio/text\"}");
        }
      }
      // If /info display information
      else if (command == "status") {
        sendInfo();
      }
      else if (command == usb_name) {
        Serial.print("OK");
      }
      else if (command == "info") {
        Serial.println(usb_name);
      }
    }
  }
}

//Display arduino code filename, url, version, wiring (tx,rx) and status (tx,rx)
//Can be display again by typing /info
void sendInfo() {
  Serial.print("{\"file\":\"radio2serial.ino\",\"url\":\"github.com/madnerdorg/radio433\",\"ver\":\"");
  Serial.print(VER);
  Serial.print("\",\"pins\":\"tx:");
  Serial.print(txPin);
  Serial.print(";rx:");
  Serial.print(rxPin);
  Serial.print("\",\"state\":\"tx:");
  Serial.print(txStatus);
  Serial.print(";rx:");
  Serial.print(rxStatus);
  Serial.println("\"}");
}


//Equivalent of explode in PHP (use for serial commands parsing)
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

/*


  Setup and Check Radio
*/

//Setup Radio Reception and Transmission
void setupRadio() {
  //Check reception
  checkRxRadio();

  //Receive 433mhzforArduino
  RemoteReceiver::init(-1, REPEATS, showOld434); //Receive old switches codes
  NewRemoteReceiver::init(-1, REPEATS, showNew434); //Receive new switches codes
  InterruptChain::setMode(0, CHANGE); //Use Interupt chains to chain Receiver
  InterruptChain::addInterruptCallback(0, RemoteReceiver::interruptHandler);
  InterruptChain::addInterruptCallback(0, NewRemoteReceiver::interruptHandler);
  InterruptChain::addInterruptCallback(0, showText434);

  //Receive RadioHead message
  driver.init();

  //If reception works we try to send and receive /radio/new/1234/on
  if (rxStatus) {
    checkTxRadio();
  }
}

//we check if rxPin is plugged.
//If digitalRead change from HIGH to LOW multiples times
//We assume the receiver is plugged.
void checkRxRadio() {
  int testRadioHigh = 0;
  int testRadioLow = 0;

  //We check what rxPin received 64 times.
  for (int i = 0; i < 64; i++) {
    if (digitalRead(rxPin)) {
      testRadioHigh++;

    }
    else
    {
      testRadioLow++;
    }
    //Serial.print(digitalRead(rxPin));
    delay(25);
  }

  //If we have only received LOW, display error and blink led fast
  if (testRadioLow == 64) {
    rxStatus = false;
    //blinkLed(25);

  } else {
    rxStatus = true; //If not put rxStatus to true.
  }

}

//To check the transmitter, we send a radiocode and
//see if we have received it
void checkTxRadio() {
  sendNew434("/radio/new/1234/0/off");

  //If we haven't receive radio code, display error and blink led slow
  if (!txStatus) { //If we haven't receive radio code
    //blinkLed(50);
  }
}


/*


  Receive radio
*/

//Show 434 code with no self-learn code
// /radio/old/code/(period)
void showOld434(unsigned long receivedCode, unsigned int period) {
  Serial.print("{\"data\":\"/radio/old/");
  Serial.print(receivedCode);

  //If period is not between 400 and 500 we display it
  if (period < 400 || period > 500) {
    Serial.print("/");
    Serial.print(period);
  }

  Serial.println("\"}");
}


//Show 434 code with self-learn code
// /radio/new/address/id/level/(period)
void showNew434(NewRemoteCode receivedCode) {
  if (receivedCode.address == 1234) {
    txStatus = true;
  } else {
    // Print the received code.
    Serial.print("{\"data\":\"/radio/new/");
    Serial.print(receivedCode.address);
    Serial.print("/");
    Serial.print(receivedCode.unit);
    Serial.print("/");

    // Display status (on/off/dim)
    switch (receivedCode.switchType) {
      case NewRemoteCode::off:
        Serial.print("off");
        break;
      case NewRemoteCode::on:
        Serial.print("on");
        break;
      case NewRemoteCode::dim:
        Serial.print(receivedCode.dimLevel);
        break;
    }

    //If period is not between 190 and 310 we display it
    if (receivedCode.period < 190 || receivedCode.period > 310) {
      Serial.print("/");
      Serial.print(receivedCode.period);
    }
    Serial.println("\"}");

    //If a test code (/radio/new/1234) is received we assume
    //transmission works correctly.
  }

}

//show 434 radioHead message
// /radio/text/string
void showText434() {

  //Create buffer to receive text message
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  if (driver.recv(buf, &buflen)) // Non-blocking Reception
  {
    char* radiocode; //Create array of char
    buf[buflen] = '\0'; //Add a end of string to buffer
    radiocode = (char *)buf; //Get uint buffer to array of char
    Serial.print("{\"data\":\"/radio/text/");
    Serial.print(radiocode); //Display text message
    Serial.println("\"}");
  }
}

/*


   Send radio
*/

//Send a text with Radiohead /radio/text/Hello world
void sendText434(String line) {

  //Get text inside a string
  String code = getValue(line, '/', 3);

  //Convert string to char array
  char msg[code.length() + 1];
  //Serial.println(code.length());

  //If message is inferior to 60 characters
  if (code.length() < 60) {

    //Convert string to char array
    code.toCharArray(msg, code.length() + 1);

    //Convert char array to uint8_t and send it
    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();

    //Show what has been sent
    Serial.print("{\"data\":\"/radio/text/");
    Serial.print(msg);
    Serial.println("\"}");
    digitalWrite(statusLedPin, LOW); //Reset status (if text was previously too long)

  } else { //if text > 60 characters
    Serial.println("{\"err\":\"Text too long\"}");
    blinkLed(100); //Blink led if text is too long

  }

}


// Send an old radio code
// /radio/old/code/(period)
void sendOld434(String line) {
  String code_temp = getValue(line, '/', 3);
  String period_temp = getValue(line, '/', 4);

  long code = code_temp.toInt();
  int period = period_temp.toInt();

  if (period == 0) {
    period = 400;
  }

  RemoteTransmitter::sendCode(txPin, code, (period - RADIOHEAD_LATENCY_CORRECTION), 3);

  //Serial.println(code);
  //Serial.println(period);
}

// Send a new radiocode
// /new/address/id/level/(period)
void sendNew434(String line) {
  String address_temp = getValue(line, '/', 3);
  String id_temp = getValue(line, '/', 4);
  String level_temp = getValue(line, '/', 5);
  String period_temp = getValue(line, '/', 6);

  //Check if we want to dim or not
  int level;
  if (level_temp == "on") {
    level = 16;
  }
  else if (level_temp == "off") {
    level = 0;
  }
  else {
    level = level_temp.toInt();
  }

  //If period was not set or badly set we set it to 260 (default)
  int period = period_temp.toInt();

  //Default period
  if (period == 0) {
    period = 200;
  }


  int id = id_temp.toInt();
  long address = address_temp.toInt();


  NewRemoteTransmitter transmitter(address, txPin, (period - RADIOHEAD_LATENCY_CORRECTION), 3);
  if (level == 0) {
    transmitter.sendUnit(id, false);
  }
  else if (level >= 1 && level <= 15) {
    transmitter.sendDim(id, level);
  }
  else if (level == 16) {
    transmitter.sendUnit(id, true);
  }

  //Serial.println(address);
  //Serial.println(id);
  //Serial.println(level);
  //Serial.println(period);
}

/*


  Led
*/

//Blink system led if something goes wrong and stay light up.
void blinkLed(int speed) {
  // Turn the LED on, then pause
  for (int i = 0; i <= NbledBlink; i++) {
    digitalWrite(statusLedPin, HIGH);
    delay(speed);
    digitalWrite(statusLedPin, LOW);
    delay(speed);
  }
  digitalWrite(statusLedPin, HIGH);
}
