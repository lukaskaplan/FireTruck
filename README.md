# FireTruck

Cardboard Fire truck powered by Arduino UNO for kids. 


## My project

<img src="/images/Project.png" width=640 alt="Cardboard fire truck - project">

## Final product

<img src="/images/FT_1.jpg" width=320 alt="Cardboard firetruck"> <img src="/images/FT_2.jpg" width=320 alt="Cardboard firetruck"> 
<img src="/images/FT_3.jpg" width=320 alt="Cardboard firetruck"> <img src="/images/FT_4.jpg" width=320 alt="Cardboard firetruck">

## Video with firetruck demonstration

<a href="http://www.youtube.com/watch?feature=player_embedded&v=PoF56u1W4I4" target="_blank"><img src="http://img.youtube.com/vi/PoF56u1W4I4/0.jpg" 
alt="Cardboard firetruck demontration" width="320" height="240" border="10" /></a>

# Technical section
## Part list
#### Connectors and cables
- 1x XLR187F
- 1x XLR188F
- 1x Panel IEC320 C14 230V
- 2x Faston female terminal 6,3x0,4 for cable 1-1,5mm2
- WAGO221-412
- WAGO221-413
- WAGO221-413
- Wire - CYA 1x0,5 H05V-K (red,blue,white,yellow)

#### Lights
- LED strip 12W/m 5730 cold white 12V - approx. 1,5m
- LED strip addressable WS2812B 9W/m 5V - min. 16 leds or 60cm

#### Inside el.
- 1x MOSFET module with led signalization
- 2x Relay module 5V with LED signalization
- 2x Speaker 40mm/8Ohm/3W
- 1x Battery Westinghouse WA1272 12V/7,2Ah F2 or similar
- 1x Switching power supply MEAN WELL ADD-55A

#### Panel
- 1x Key switch on/off (ignition switch)
- 1x Arcade push button 30mm - blue
- 1x Arcade push button 30mm - white
- 1x Cradle switch (backlight 12V) - red
- 1x Cradle switch (backlight 12V) - blue
- 1x Cradle switch (backlight 12V) - green
- 1x Panel ampermeter 0-3A
- 1x Panel voltmeter 0-15V

#### Arduino + shield
- 1x Arduino UNO R3
- 1x PCB blank shield for arduino uno (or create your own PCB)
- 1x 6-pin female pin header with long legs
- 2x 8-pin female pin header with long legs
- 1x 10-pin female pin header with long legs
- 7x 2-pin Screw terminal (pin distance 5,08mm)
- 1x C1 1000uF/16V
- 1x C2 470uF/16V
- 3x resistor 10K/0,25W (R1,R2,R3)
- 1x resistor 640 (R4)
- 1x resistor 330 (R5)
- 2x resistor 1K (R6,R7)
- 1x optocoupler PC817
- 2x 8-pin female pin header
- 2x DFPlayer mini
- 2x micro SD card

## Wiring diagram

![Firetruck - Wiring diagram](schematics/FireTruck-schematics.png "Firetruck - wiring diagram")

[Firetruck - schematic PDF](schematics/FireTruck-schematics.pdf)

## Arduino UNO shield

![Firetruck - arduino shield schematic](arduino-shield/Schematic_Firetruck&#32;-&#32;arduino&#32;uno&#32;shield.png "Firetruck - arduino shield schematic")

[Firetruck - arduino shield schematic PDF](arduino-shield/Schematic_Firetruck&#32;-&#32;arduino&#32;uno&#32;shield.pdf)

![Firetruck - arduino shield PCB](arduino-shield/PCB_all.png "Firetruck - arduino shield schematic")

[Firetruck - arduino shield PCB PDF](arduino-shield/PCB_botom.pdf)


## Construction details

#### Front lights

Transparent cover is from sushi box, yellow direction ligts are from yoghurt cup. White LED strips:

<img src="/images/frontlights1.jpg" width=320 alt="Firetruck front lights"> <img src="/images/frontlights2.jpg" width=320 alt="Firetruck front lights - detail">

#### Top beacon

Two blue PET bottles and addresable RGB LED strip divided to two parts. Small speaker for sound:

<img src="/images/top-beacon1.jpg" width=320 alt="Firetruck top beacon"> <img src="/images/top-beacon2.jpg" width=320 alt="Firetruck top beacon"> <img src="/images/top-beacon3.jpg" width=320 alt="Firetruck top beacon"> 

#### Rear ligts

Two red PET bottles with common white LED strip inside:

<img src="/images/rear-lights1.jpg" width=320 alt="Firetruck rear ligts"> <img src="/images/rear-lights2.jpg" width=320 alt="Firetruck rear ligts"> 

#### Rear beacons

Inside part is made from two PET bottle caps. They are holding 3 LEDS of addressable RGB strip. We can create rotating effect by blinking one by one. Beacons are covered by bottom of 0.5l blue PET bottle.

<img src="/images/rear-beacons1.jpg" width=320 alt="Firetruck rear beacons"> <img src="/images/rear-beacons2.jpg" width=320 alt="Firetruck rear beacons"> <img src="/images/rear-beacons3.jpg" width=320 alt="Firetruck rear beacons">

#### Connectors

C14 connector (230V) is used for charging from wall outlet. XLR connector is used for connection of truck trailer.

<img src="/images/connectors.jpg" width=320 alt="Firetruck connectors">

#### Arduino shield

I made it from blank arduino uno shield PCB and wires because of fast prototyping. It is not nice, but it works...

<img src="/images/shield-top.jpg" width=320 alt="Firetruck arduino shield"> <img src="/images/shield-bottom.jpg" width=320 alt="Firetruck arduino shield">

There are two DFPlayers on the shield. Pinout of DFPlayer mini:

<img src="/images/DFPlayer-mini.jpg" width=240 alt="Firetruck dfplayer mini pinout">

(Be careful, when you will use PCB design from this repo, there are DFPlayers upside down compared to my photos - you will put SD cards from oposite side. VCC pin of DFPlayer should be connected to square pin.)
