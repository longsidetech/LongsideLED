# LongsideLED

This is a speedy way of building a scrolling LED sign using 8x8 led displays using the MAX7219 displays and ESP8266.  It was build originally to go in the window of studio 1 at Longside Radio ( http://longsideradio.co.uk ) and to be a simple way of displaying and updating a scrolling message.

Features : 
* Written using the Arduino IDE
* Captive portal for WiFI setup
* IP address displayed on boot
* Default message displayed on boot
* Can change the message from a web page hosted on the ESP8266
* Can clear the display so it no longer displays a message or revert to default.

# Hardware

I suggest you use a NodeMCU dev kit type board or Weemos D1 mini.  You can do this with a bare board but if you are doing that I assume you know what you are doing and can adapt this to your needs.   The following setup works here but I accept no liability for damage, loss or anything really :)  Any suggestions of improvements a gladly recieved. 

The MAX7219 modules just daisy chain together with the output  of the first one going to the inputs of the second one.  It is important to note that your input starts at the right hand end of the modules.  The left hand output pins are not connected to anything.  The right hand input pins are connected to:
* VCC --> 5V PIN
* GND --> GND PIN
* DIN --> D7 PIN
* CS  --> D4 PIN
* CLK --> D5 PIN

If you are using the 4 modules that are already joined together style (with the big MAX7219 not visible) then connect in the same way but follow the instructions in the code for downloading the forked version of the library with rotation support. 

I have tested this with upto 8 displays with a Lolin nodemcu ish type board and it seems to work.  I suspect the limit would be what you can draw from the voltage regulator.  You could probably up this by powering the VCC rail seperatly and not from the ESP but if you do that you will probably need to common the negative rail and we are getting out of the scope of these instructions and you are on your own! Also bear in mind that if you are doing this while you are only plugged into your PC then you are limited to the 500ma that your onboard USB can put out! 

# Software

I am assuming you have the Arduino IDE installed and the ESP8266 libraries installed too.  If you haven't there are lots of tutorials on this available.

Download the repository from GitHub and also download and install the awesome LedMatrix library from here : https://github.com/squix78/MAX7219LedMatrix you will also need the very very awesome Wifimanager library : https://github.com/tzapu/WiFiManager

Before you flash the code in the Arduino IDE scroll to the config section and update the number of modules you are using and the default message you want to display and the captive portal access point name. 

# Booting

On boot if your ESP8266 board has not got a Wifi access point stored in it then it will put itself into AP mode and behave as a captive portal for you to configure your wifi settings (I did say the Wifimanager library is very very awesome).     Once you have done this it will reboot and scroll it's IP address across the display.   Go to this IP address and have fun! 

Don't be alarmed if all of the LEDs (or some of them) light up fully on boot for a second or two... not sure why it does this but it does...


