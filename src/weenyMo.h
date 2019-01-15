/*
 MIT License

Copyright (c) 2019Phil Bowles <esparto8266@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef weenyMo_H
#define weenyMo_H
#include <ESP8266WiFi.h>
#include <ESPAsyncUDP.h>
#include <ESPAsyncWebServer.h>
#include <string>
#include <functional>
using namespace std;
using namespace std::placeholders;

class weenyMo: public AsyncWebServer {
    String      reply     ="HTTP/1.1 200 OK\nCACHE-CONTROL: max-age=60\nLOCATION: http://%ip%:80/wemo\nST: urn:Belkin:device:**\nUSN: uuid:%usn%\n\n"; 
    String      xml       = "<?xml version=\"1.0\"?><root><device><deviceType>urn:Belkin:device:controllee:1</deviceType><friendlyName>%fn%</friendlyName>"
                          "<manufacturer>Belkin International Inc.</manufacturer><modelName>weenyMo</modelName><modelNumber>1</modelNumber><UDN>uuid:%usn%</UDN></device></root>"; 
    AsyncUDP              udp;
    function<void(bool)>  cmd;
    string                name;
  public:
    void gotIPAddress(){     
      char usn[14];
      sprintf(usn,"weeny-%06X",ESP.getChipId());
      
      reply.replace("%ip%",WiFi.localIP().toString().c_str());     
      reply.replace("%usn%",usn);
      xml.replace("%fn%",name.c_str());
      xml.replace("%usn%",usn); 
                       
      on("/wemo", HTTP_GET,bind([](AsyncWebServerRequest *request,String x){ request->send(200, "text/xml", x); },_1,xml));

      on("/upnp", HTTP_POST,
         [](AsyncWebServerRequest *request){ request->send(200, "text/htm", "OK"); },NULL,
         [this](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total){              
              char* p = (char*)malloc(len+1);
              memcpy(p,data,len);
              p[len]='\0';
              String s(reinterpret_cast<char *>(p));
              free(p);
              if(s.indexOf("SetBinary")!=-1) cmd(s.indexOf(">1<")!=-1 ? true:false);
      });
      
     if(udp.listenMulticast(IPAddress(239,255,255,250), 1900)){
        udp.onPacket( bind([](AsyncUDPPacket packet,String reply){
              byte* p = (byte*)malloc(packet.length()+1);
              memcpy(p,packet.data(),packet.length());
              p[packet.length()]='\0';
              String msg=reinterpret_cast<char*>(p);
              free(p);           
              if(msg.indexOf("M-SEARCH")!=-1 && msg.indexOf("Belkin")!=-1) packet.printf(reply.c_str(), reply.length());                   
         },_1,reply));                  
    }
    begin();
    Serial.printf("TELL ALEXA TO DISCOVER DEVICES\n");              
 };

    weenyMo(const char* friendlyName,function<void(bool)> onCommand): AsyncWebServer(80),cmd(onCommand),name(friendlyName){}              
}
#endif // weenyMo_H