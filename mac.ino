#include <ESP8266WiFi.h>
void setup(){
Serial.begin(115200);
Serial.println();
Serial.print("ESP8266 Board MAC Address: ");
Serial.println(WiFi.macAddress());
}

void loop()
{

}
//ESP8266 Board MAC Address: 4C:11:AE:0A:0C:3B me

//ESP8266 Board MAC Address: 80:7D:3A:25:BB:F5 
//ESP8266 Board MAC Address:2C:F4:32:30:BF:FA //8