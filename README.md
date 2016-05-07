# ESP8266-base-platformio-project
A starting point for ESP8266 and Platformio. Things to achieve here:

1. Over the Air (ATA) updates
2. Access point config
  * if not connected to wifi, enter access point (ap) mode
  * user can set wifi settings in ap mode and then reset the ESP
  * another input can reset the wifi settings - optional
3. other things I'm not thinking of right now...

### This is a [Platformio](http://platformio.org/) project:

to build...

    platformio run
    
to upload...

    platformio run --target upload

to start a serial monitor...

    platformio serialports monitor -b 115200
    
update the ota IP address - use your router to set a static IP based on MAC - in platformio.ini

    # for over the air - set port to ip address of module...
    upload_port = 10.0.2.240