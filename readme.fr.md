[English version](https://github.com/madnerdorg/radio433/)

Radio433
----
![radio2serial](https://github.com/madnerdorg/radio433/blob/master/doc/radio2serial.jpg)        
Cette arduino permet de recevoir et envoyer plusieurs types de communications radio en 433Mhz           
Protocôles géré: RemoteTransmitter (old)/NewRemoteTransmitter (new)/RadioHead (text)      

* Contrôler des prises de courants à bas prix    
* Envoyer du texte entre des arduino/raspberry pi   

![new](https://github.com/madnerdorg/radio433/blob/master/doc/new.png)   
![old](https://github.com/madnerdorg/radio433/blob/master/doc/old.png)   
Pour plus d'informations sur les appareils compatibles: https://bitbucket.org/fuzzillogic/433mhzforarduino/    

Cette technologie n'est pas très fiable mais est utile si vous voulez éteindre/allumer des prises de courants à distance pour pas cher (utiliser des rfm69/95 si vous voulez une communication plus sécurisé/fiable)      

# Instructions
* Copier **radio433 / libraires** dans votre carnet de croquis arduino (Documents/Arduino)
* Téléverser **radio433/radio433.ino**   
* Aller sur [TODO]   
  
# Commandes disponibles
* /info : Affiche nom:port (radio433:42002)  
* /status : Affiche l'état (en json)  
```
{"file":"radio2serial.ino","url":"github.com/madnerdorg/radio433","ver":"1.2","pins":"tx:10;rx:2","state":"tx:1;rx:1"}
```
## Recevoir des codes radio
Les données reçus sont en JSON (ex: "{"data" : "/radio/text/Hello World"}")

## Envoyer des codes radio
Envoyer un messagee RadioHead (virtualwire)
```
/radio/text/Hello World
```
Allumer un appareil radio "new" "addresse:1234 unité:1" 
```
/radio/new/1234/1/on
```
Eteindre un appareil radio "new" "addresse:1234 unité:0"
```
/radio/new/1234/0/off
```
Règler la luminosité à 8/16 "addresse:1234 unité:2"
```
/radio/new/1234/2/8
```
Envoyer un code "old" "123456"
```
/radio/old/123456
```

# Composants
* Receiver : ASK Receiver Super-heterodyne  OOK RF - 4€
* Transmitter : ASK transmitter - 2€
* Arduino nano ch340g - 3€
Coût total: 10€

# Branchements
![Pinout](https://raw.githubusercontent.com/madnerdorg/radio433/master/doc/schema_radio2serial.png)
![radio2serial_internal](https://github.com/madnerdorg/radio433/blob/master/doc/radio2serial_internal.jpg)

# Modèles 3D
[Afficher les modèles](https://github.com/madnerdorg/radio433/tree/master/3D)

# Licenses

radio433.ino      

|Auteur                 | Rémi Sarrailh                                    |
|:---------------------:|:------------------------------------------------:|
|License                | MIT                                              |

RadioHead Library      

|Auteur                 | Ayrspace                                         |
|:---------------------:|:------------------------------------------------:|
|License                | MIT                                              |
|Donation               | http://www.airspayce.com                         |
|Documentation (ENGLISH)| http://www.airspayce.com/mikem/arduino/RadioHead/|


433MhzForArduino Library     

|Auteur                 | Randy Simons                                                |
|:---------------------:|:-----------------------------------------------------------:|
|License                | MIT                                                         |
|Donation               | Bitcoin: 1Ar433MfHWV7a4yGj3avg3dpTRzHGvT4PP                 |
|Documentation (ENGLISH)| https://bitbucket.org/fuzzillogic/433mhzforarduino/         |

