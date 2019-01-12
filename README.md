# weenymo
weenyMo is (yet another)Belkin Wemo emulator, providing Alexa voice control for your ESP8266 (in Arduino IDE) in about the smallest "footprint" you will find

## WHY? 
There's Fauxmo and others, but they all seem to send more data than they need to Alexa, requiring big data strings and more complexity than absolutely required. Let's face it: 90% of IOT devices just switch on or off... Why then have more code than you need? I decided to see how much could be cut away while still providing "switch on" / "switch off" functionality

Here's another reson: this is it, all there is to it: Once object constructor and one function.
```cpp
#include<weenyMo.h>
//
// This function will be called when Alexa hears "switch [on | off] 'tiny tim'" 
// ESP8266 builtin LED is "backwards" i.e. active LOW, hence Alexa's ON=1 needs reversing
//
void onVoiceCommand(bool onoff){ digitalWrite(BUILTIN_LED,!onoff);  } 
//
// The only constructor: const char* name, function<void(bool)> onCommand
// choose the name wisely: Alexa has very selective hearing!
//
weenyMo("tiny tim",onVoiceCommand);
           
void setup() {
  Serial.begin(74880);
  pinMode(BUILTIN_LED,OUTPUT);
  digitalWrite(BUILTIN_LED,HIGH); // start OFF 
  WiFi.begin(SSID,PWD); // replace OF COURSE
  WiFi.waitForConnectResult();  // so much neater than those stupid loops and dots
  weenyMo.gotIPAddress(); // ready to roll...Tell Alexa to discover devices.
}

void loop() {}

```

That really is all there is to it. If you know of or find anything easier / smaller - please let me know!

© 2019 Phil Bowles
* philbowles2012@gmail.com
* http://www.github.com/philbowles
* https://8266iot.blogspot.com
* https://www.facebook.com/groups/esp8266questions/
