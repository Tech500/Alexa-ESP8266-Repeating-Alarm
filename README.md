# Alexa-ESP8266-Repeating-Alarm
Alarm project utilizing Alexa, ESP8266 and a contact sensor for a Door or for a Window.

“Sinric Pro” allows for interfacing ESP32/ESP8266 microcontrollers and includes a Github repository for a “Arduino C++” library.   Skill can be enabled from Amazon, for “Alexa” it is a free Skill:  https://www.amazon.com/HOME-Sinric-Pro/dp/B07ZT5VDT8 .  
Sixteen “Arduino” library example Sketches are included.  Devices that could be attached to esp32/ESP8266 microcontrollers; for example, a Contactsensor is a device used on a doors or windows to detect open or closed condition.  Project uses an “Alexa Echo,” an ESP8266, a Contactsensor, and a Resistor.  Circuit using the contactsensor was assembled on breadboard with Dupont jumper wires, a ESP8266, and a common contact sensor https://www.amazon.com/NTE-Electronics-54-627-Magnetic-Terminals/dp/B007Z7OGQU for development of the modified code.  
“Contactsensor.ino” example code was provided in the “Sinric Pro,” Github repository: https://github.com/sinricpro/esp8266-esp32-sdk
First, you will need to register for a Sinric Pro account.  Your first five devices are free.  If you need more devices, it's only $3 per device for a year.
Setting up “Alexa”:  jumping to Routines, two routine were setup. One is called “Announce Open,” the second is called “Announce Closed.”  Setup “Announce Open” by tapping the plus sign in the upper right corner of the fist Routine screen.  Tap plus sign next to “Enter routine name.”  Give routine a name you can associate with the routine.  Next, tap plus sign beside “When this happens,” tap on “Smart Home” then tap on your device name for ESP32/ESP8266 device.  Next tap on “Open,” then in the upper, right corner tap next.”  “New Routine” screen; “When” will show device name when opened.  Leave default value “Anytime.”  Now we will “Add Action,” there are two actions I added: one is “Alexa Says” which has been customized by tapping “Alexa Says,” then tap “Customized” on the “Alexa Says” screen.  Next, tap on “Alexa Say,” then tap “Customize” and type “Garage Door opened” without any special characters; in the upper, right corner tap next.   Confirm what “Alexa” is going to say by tapping in the upper, right corner and save.   Back on the “New Routine” screen tap “Choose Device.” Here we will select from which “Amazon” device “Alexa” will say “Garage door open” and will play the “Red-Alert” sound.
Second “Routine” to setup is less complex.  Setup “Announce Closed” by tapping the plus sign in the upper right corner of the fist Routine screen.  Tap plus sign next to “Enter routine name.”  Give routine name you can associate with the routine.  Next, tap plus sign beside “When this happens,” tap on “Smart Home” then tap on your device name for ESP32/ESP8266 device, next tap on “Close,” then in the upper, right corner tap next.”  “New Routine” screen; “When” will show device name opens.  Leave default value “Anytime.”  Now we will “Add Action,” tap on “Alexa Say,” then tap “Customize” and type “Garage Door closed” without any special characters; in the upper, right corner tap next.   Confirm what “Alexa” is going to say by tapping in the upper, right corner.  Next, having confirmed what “Alexa” says; save routine by tapping in the upper, right corner “Save.”  This completes setting up routines used in this project.

“Fritzing” wiring diagram:
 

Project uses modified version of the “contactsensor.ino” example from “Sinric Pro’s, Github repository:
Heart of the project is in the “Arduino” loop; additional code placed in the loop:

if ((digitalRead(CONTACT_PIN) == LOW) && ((MINUTE % 10) == 0)) 
{
        if(SECOND < 2)    
        {  
            SinricProContactsensor &myContact = SinricPro[CONTACT_ID]; // get contact sensor device
            myContact.sendContactEvent(actualContactState);      // send event with actual state
            Serial.println( "actualContactState:  " + (String)actualContactState);

            Serial.println("");
            Serial.println(dtStamp);
            Serial.println("");
         }                     
  }

“if ((digitalRead(CONTACT_PIN) == LOW) && ((MINUTE % 10) == 0))“ condition of the “CONTACT_PIN,” which is defined as pin 4  is “LOW,” and the “MINUTE” = 10, 20, 30, 40, 50, or 60 (MINUTE  %  10  ==  0) then continues.  The next “if” condition limits the number of passes that are processed.  
Upon completion of the project; Alexa can be asked:” Alexa being in the Group “Garage”, “Alexa is the Garage open or closed.”    “Alexa” can also be asked: “Alexa is device name opened   or closed. “ Additionally, every ten minutes there is a “Red-Alert” alarm; that was created earlier in the “Announce Open” routine.   Alarm will sound as long as the contactsensor is “open.”
More code was added the Sketch for NTP Time server protocol, over UDP; which requires using the “Arduino,” WiFiUdp library.  This added code is commented in the modified Sketch. 
Total Project cost is under $10.00, if you already have one of the “Amazon Echo” devices “Amazon Alexa” devices for purchase:  https://www.amazon.com/s?k=alexa+devices&ref=nb_sb_noss_1 



