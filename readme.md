[French version / Version française](https://github.com/madnerdorg/radio433/blob/master/readme.fr.md)

Radio433
----
![radio2serial](https://github.com/madnerdorg/radio433/blob/master/doc/radio2serial.jpg)        
This arduino can send and receive multiples 433Mhz (or 315Mhz) types of radio communications.      
Protocols managed: RemoteTransmitter (old)/NewRemoteTransmitter (new)/RadioHead (text)      

* Control Cheap Power Outlets    
* Send text between arduino/raspberry pi   

![new](https://github.com/madnerdorg/radio433/blob/master/doc/new.png)   
![old](https://github.com/madnerdorg/radio433/blob/master/doc/old.png)   
For more information on compatible devices: https://bitbucket.org/fuzzillogic/433mhzforarduino/    

This technology (433Mhz) isn't really reliable but this is pretty useful if you want to turn on/off power outlets remotely and cheaply (use rfm69/95 if you want secure communication)      

# Instructions
* Copy **radio433 / libraires** into arduino sketchbooks (Documents/Arduino)    
* Upload **radio433/radio433.ino**   
* Go to http://madnerd.org/interface/homeautomation 
  
# Commands available
* /info : Display name:port (radio433:42002)  
* /status : Display status (in json)  
```
{"file":"radio2serial.ino","url":"github.com/madnerdorg/radio433","ver":"1.2","pins":"tx:10;rx:2","state":"tx:1;rx:1"}
```
## Receive radio
Data are received in JSON (ex: "{"data" : "/radio/text/Hello World"}")

## Send radio
send RadioHead code "Hello world"
```
/radio/text/Hello World
```
turn on new radio "address:1234 unit:1" 
```
/radio/new/1234/1/on
```
turn off new radio "address:1234 unit:0"
```
/radio/new/1234/0/off
```
dim new radio to 8/16 "address:1234 unit:2"
```
/radio/new/1234/2/8
```
send old radio code "123456"
```
/radio/old/123456
```

# Components
* Receiver : ASK Receiver Super-heterodyne  OOK RF - 4€
* Transmitter : ASK transmitter - 2€
* Arduino nano ch340g - 3€
Cost: 10€

# Wiring
![Pinout](https://raw.githubusercontent.com/madnerdorg/radio433/master/doc/schema_radio2serial.png)
![radio2serial_internal](https://github.com/madnerdorg/radio433/blob/master/doc/radio2serial_internal.jpg)

# 3D models
[Show Models](https://github.com/madnerdorg/radio433/tree/master/3D)

# Licences

radio433.ino      

|Author                 | Rémi Sarrailh                                    |
|:---------------------:|:------------------------------------------------:|
|License                | MIT                                              |

RadioHead Library      

|Author                 | Ayrspace                                         |
|:---------------------:|:------------------------------------------------:|
|License                | MIT                                              |
|Donation               | http://www.airspayce.com                         |
|Documentation (ENGLISH)| http://www.airspayce.com/mikem/arduino/RadioHead/|


433MhzForArduino Library     

|Author                 | Randy Simons                                                |
|:---------------------:|:-----------------------------------------------------------:|
|License                | MIT                                                         |
|Donation               | Bitcoin: 1Ar433MfHWV7a4yGj3avg3dpTRzHGvT4PP                 |
|Documentation (ENGLISH)| https://bitbucket.org/fuzzillogic/433mhzforarduino/|

