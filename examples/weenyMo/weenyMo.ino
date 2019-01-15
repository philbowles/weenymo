#include<weenyMo.h>
//
//  choose your name carefully: alexa has very selective hearing!
//
weenyMo("tiny tim",[](bool onoff){ digitalWrite(BUILTIN_LED,!onoff);  });
//
// if you dont like lambda functions:
//
/*
void onVoiceCommand(bool onoff){
  digitalWrite(BUILTIN_LED,!onoff);  
}

weenyMo("tiny tim",onVoiceCommand);
 */            
void setup() {
  Serial.begin(74880);
  pinMode(BUILTIN_LED,OUTPUT);
  digitalWrite(BUILTIN_LED,HIGH); // start OFF 
  WiFi.begin(SSID,PWD);
  WiFi.waitForConnectResult();
  weenyMo.gotIPAddress();
}

void loop() {}
