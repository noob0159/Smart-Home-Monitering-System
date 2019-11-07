/*macro definitions of PIR motion sensor pin and LED pin*/
#include <WiFi.h>
#include "time.h"
int motion,pressure = 0;
int flag = 0;
int a = 0;
int c = 0;
#define touch 27
 uint32_t start,stoptime = 0;
#define PIR_MOTION_SENSOR 4//Use pin 2 to receive the signal from the module
#define AP_SSID "nana"
#define AP_PSW  "pakistan1947" 
const IPAddress host(192,168,1,103); //欲访问的地址
uint16_t port = 50037;         //服务器端口号
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 25200;
const int   daylightOffset_sec = 3600;
WiFiClient client;   //创建一个tcp client连接

char date[50];

void setup()
{
    pinMode(PIR_MOTION_SENSOR, INPUT);
    pinMode(touch, INPUT);
    Serial.begin(115200);  
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false); 
    WiFi.begin(AP_SSID,AP_PSW);
    Serial.print("Wait for WiFi... ");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi connected");
 configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
 delay(1000);
 printLocalTime();
 //state(VRMS,current1);

}

void loop()
{
  
    motion = digitalRead(PIR_MOTION_SENSOR);
   delay(500);
    pressure = digitalRead(touch);
    Serial.print(motion);
    Serial.print(pressure);
    Serial.print("\n");

       if(motion == 1 && pressure == 0)
       {
        delay(500);
        Serial.println("im here"); }
       if (pressure == 1 && motion == 0)
         {   
//          if(client.connect(host,port))
//          {
         Serial.println("person left the room"); 
//         client.println ("person left the room");  
//         client.print(date);
         printLocalTime(); 
//         client.stop();   
        }
       
   else if ( motion == 0 && pressure == 1 )
      {
        delay(500);
        Serial.println("im in"); 
        }
       if (pressure == 0 && motion == 1)
      {
//       if(client.connect(host,port))
//       {
        Serial.println("person came back to room"); 
//        client.println("person came back to room");
//        client.println(date);
          printLocalTime(); 
//        client.stop(); 
        //a = 0; 
        }
    }
  
void printLocalTime()
{
  time_t t;
  struct tm *tm;
  t = time(NULL);
  tm = localtime(&t);
  static const char *wd[7] = {"Sun","Mon","Tue","Wed","Thr","Fri","Sat"};
  if(!localtime(&t)){
    Serial.println("Failed to obtain time");
    return;
  }
 // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.printf("%04d/%02d/%02d(%s) %02d:%02d:%02d\n",
        tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
        wd[tm->tm_wday],
        tm->tm_hour, tm->tm_min, tm->tm_sec);
 sprintf(date,"%04d/%02d/%02d(%s) %02d:%02d:%02d\n",tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
        wd[tm->tm_wday],
        tm->tm_hour, tm->tm_min, tm->tm_sec);
 //Serial.print(date);
}
