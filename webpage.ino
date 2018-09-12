/*
 * RFID ----> Wemos
 * RST -> D3
 * SCK -> D5
 * MISO -> D6
 * MOSI -> D7
 * SDA -> D8
 * Switch -> D2
 */
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN D8 
#define RST_PIN D3 
#define PIN D1 
byte c=false;
int x = true;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, PIN, NEO_GRB + NEO_KHZ800);
MFRC522 mfrc522(SS_PIN, RST_PIN);       
MFRC522::MIFARE_Key key;
const IPAddress AP_IP(192, 168, 1, 1);
const char* AP_SSID = "MARS_ROVER";
String t;
int space= 32;
char ascii_space= char(space);
boolean SETUP_MODE;
String SSID_LIST;
DNSServer DNS_SERVER;
char read_team[32];
ESP8266WebServer WEB_SERVER(80);
String DEVICE_TITLE = "MARS ROVER";
boolean POWER_SAVE = false;
int switchState,lastswitchState=LOW;
int a,b=0;
int block=38;
byte readbackblock[18];
int Finalscore,Finalscore_address=20,Finalscore_Read;
struct Resource {
  int count;
  int address;
  int score;
  int point;
  int Read;
};
struct Resource Water; 
struct Resource Gas;
struct Resource Titanium;
struct Resource Diamond;
byte nuidPICC[4];
byte memuid[11][4];
int q=0;
unsigned long w;


void setup() {
pinMode(D2,INPUT);
EEPROM.begin(512);
Serial.begin(115200);       
SPI.begin();               
mfrc522.PCD_Init();
Water.count = 0;
Water.address = 0;
Water.score;
Water.point=10;
Water.Read;

Gas.count = 0;
Gas.address = 25;
Gas.score;
Gas.point=7;
Gas.Read;

Titanium.count = 0;
Titanium.address = 10;
Titanium.score;
Titanium.point=30;
Titanium.Read;

Diamond.count = 0;
Diamond.address = 15;
Diamond.score;
Diamond.point=45; 
Diamond.Read;      
for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
strip.begin();
strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  if(q==1 && (millis()-w>3000)){
  strip.setPixelColor(0, 0, 0, 0);
  strip.show();
  strip.setPixelColor(1, 0, 0, 0);
  strip.show();
  strip.setPixelColor(2, 0, 0, 0);
  strip.show();
  q=0;
  }
  switchState=digitalRead(D2);
  if(switchState != lastswitchState){
    setupMode();
    startWebServer(switchState);
    }
  DNS_SERVER.processNextRequest();
  WEB_SERVER.handleClient();
  lastswitchState=switchState;
  if(switchState == HIGH){
    if (  mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    for (byte i=0;i<11;i++){
      for (byte j=0;j<1;j++){
        if (memuid[i][j+0] == mfrc522.uid.uidByte[j+0] &&
            memuid[i][j+1] == mfrc522.uid.uidByte[j+1] &&
            memuid[i][j+2] == mfrc522.uid.uidByte[j+2] &&
            memuid[i][j+3] == mfrc522.uid.uidByte[j+3]){
          Serial.println("Same");
          c= false;
          x= false;
          }
        else {
          if(x == false){
              break;
            }
            else{
              c=true;
              Serial.println("Not same");
              }
            }
      } 
    }
    x= true;
    if ( c == true ){
      strip.setPixelColor(0, 255, 0, 0);
      strip.show();
      strip.setPixelColor(1, 255, 0, 0);
      strip.show();
      strip.setPixelColor(2, 255, 0, 0);
      strip.show();
      q=1;
      w=millis();
    for(a=0;a<4;a++){
       memuid[b][a]=mfrc522.uid.uidByte[a];
    }
    b++;
        for(int i=0; i<11; i++) {
      for(int j=0;j<4;j++) {
         Serial.print(memuid[i][j]);
         if(j==4){
            Serial.print("\n");
         }
      }
   }
        Serial.print("\n");
        readBlock(block, readbackblock);
  Serial.print("read block: ");
  for (int j=0 ; j<16 ; j++)
  {
         Serial.write (readbackblock[j]);
  }
         Serial.println(""); 
  String myString = String((char *)readbackblock);
  if(myString == "Water"){
    Water.count++;
  EEPROM.put(Water.address,Water.count); 
  EEPROM.commit();
  Serial.print("EEPROM contents at Water Address is  : ");
  EEPROM.get(Water.address,Water.Read);
  Serial.println(Water.Read);
    }
   else if(myString == "Gas"){
    Gas.count++;
  EEPROM.put(Gas.address,Gas.count); 
  EEPROM.commit();
  Serial.print("EEPROM contents at Gas Address is  : ");
  EEPROM.get(Gas.address,Gas.Read);
  Serial.println(Gas.Read);
    }
  else if(myString == "Titanium"){
    Titanium.count++;
  EEPROM.put(Titanium.address,Titanium.count); 
  EEPROM.commit();
  Serial.print("EEPROM contents at Titanium Address is  : ");
  EEPROM.get(Titanium.address,Titanium.Read);
  Serial.println(Titanium.Read);
    }
  else if(myString == "Diamond"){
    Diamond.count++;
  EEPROM.put(Diamond.address,Diamond.count); 
  EEPROM.commit();
  Serial.print("EEPROM contents at Diamond Address is  : ");
  EEPROM.get(Diamond.address,Diamond.Read);
  Serial.println(Diamond.Read);
    }
    Water.score= Water.count * Water.point;
    Gas.score= Gas.count * Gas.point;
    Titanium.score= Titanium.count * Titanium.point;
    Diamond.score= Diamond.count * Diamond.point;
    Finalscore=Water.score+Gas.score+Titanium.score+Diamond.score;
    Serial.print("Water:");Serial.print("\t");Serial.print("\t");Serial.println(Water.score);
    Serial.print("Gas:");Serial.print("\t");Serial.print("\t");Serial.println(Gas.score);
    Serial.print("Titanium:");Serial.print("\t");Serial.println(Titanium.score);
    Serial.print("Diamond:");Serial.print("\t");Serial.println(Diamond.score);
    Serial.println("------------------------------");
    Serial.print("Finalscore:");Serial.print("\t");Serial.println(Finalscore);
    EEPROM.put(Finalscore_address,Finalscore); 
    EEPROM.commit();
    
        c= false;
    }
  // Halt PICC
  mfrc522.PICC_HaltA();

  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1(); 
  }
    }
}

void initHardware()
{
  // Serial and EEPROM
  
  
  delay(100);
}

boolean checkWiFiConnection() {
  int count = 0;
  Serial.print("Waiting to connect to the specified WiFi network");
  while ( count < 30 ) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println();
      Serial.println("Connected!");
      return (true);
    }
    delay(500);
    Serial.print(".");
    count++;
  }
  Serial.println("Timed out.");
  return false;
}

void setupMode() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  delay(100);
  Serial.println("");
  for (int i = 0; i < n; ++i) {
    SSID_LIST += "<option value=\"";
    SSID_LIST += WiFi.SSID(i);
    SSID_LIST += "\">";
    SSID_LIST += WiFi.SSID(i);
    SSID_LIST += "</option>";
  }
  delay(100);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(AP_IP, AP_IP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(AP_SSID);
  DNS_SERVER.start(53, "*", AP_IP);
  Serial.print("Starting Access Point at \"");
  Serial.print(AP_SSID);
  Serial.println("\"");
}

void startWebServer(int state) {
  if(state == HIGH){
    Serial.print("Starting Web Server at IP address: ");
    Serial.println(WiFi.softAPIP());
    // Settings Page
    WEB_SERVER.on("/", []() {
      String s= "<h2>Player Details</h2>";
      s +="<p>Enter the Team_Name and submit.</p>";
      s += "<form method=\"get\" action=\"setup\">";
      s +="Team_Name: <input name=\"Team_Name\" length=64 type=\"Team_Name\"><br><br>";
      s +="<input type=\"submit\"></form>";
      WEB_SERVER.send(200, "text/html", makePage("MARS-ROVER", s));
    });
     WEB_SERVER.on("/setup", []() {
     for (int i = 0; i < 64; ++i) {
        EEPROM.put(i, ascii_space);
        EEPROM.commit();
      }
      String Team_Name = urlDecode(WEB_SERVER.arg("Team_Name"));
      Serial.print("Team_Name: ");
      Serial.println(Team_Name);
      Serial.println("Writing Team_Name to EEPROM...");
      for (int i = 0; i < Team_Name.length(); ++i) {
        EEPROM.write(32 + i, Team_Name[i]);
      }
      EEPROM.commit();
      Serial.println("Write EEPROM done!");
      String s = "<h1>You are ready to go!!!!!!!!!</h1>";
      //s += "<p>Team_Name is: ";
      //s += Team_Name;
      s += "<p>Click below and Slide Switch after play.";
      s += "<p><a href=\"/settings\">Result Page</a></p>";
      WEB_SERVER.send(200, "text/html", makePage("MARS-ROVER", s));
      //ESP.restart();
    });

    /*WEB_SERVER.onNotFound([]() {
      String s = "<h1>WiFi Configuration Mode</h1>";
      s += "<p><a href=\"/settings\">Wi-Fi Settings</a></p>";
      WEB_SERVER.send(200, "text/html", makePage("Access Point mode", s));
    });*/
  }
  else if(state == LOW) {
    Serial.print("Web Server started at IP address: ");
    Serial.println(WiFi.softAPIP());
    EEPROM.get(Water.address,Water.Read);
    EEPROM.get(Gas.address,Gas.Read);
    EEPROM.get(Titanium.address,Titanium.Read);
    EEPROM.get(Diamond.address,Diamond.Read);
    EEPROM.get(Finalscore_address,Finalscore_Read);  
    if(char(Water.Read)== ascii_space){Water.Read=0;} 
    if(char(Gas.Read)== ascii_space){Gas.Read=0;} 
    if(char(Titanium.Read)== ascii_space){Titanium.Read=0;}  
    if(char(Diamond.Read)== ascii_space){Diamond.Read=0;}  
    if(char(Finalscore_Read)== ascii_space){Finalscore_Read=0;}   
    Serial.print("EEPROM contents at Titanium Address is  : ");
    Serial.println(Titanium.Read);
    Serial.print("EEPROM contents at Water Address is  : ");
    Serial.println(Water.Read);
    Serial.print("EEPROM contents at Gas Address is  : ");
    Serial.println(Gas.Read);
    Serial.print("EEPROM contents at Diamond Address is  : ");
    Serial.println(Diamond.Read);
     WEB_SERVER.on("/settings", []() {
      String s= "<h3>Result Time</h3>";
     
      s+="<style>";
      s+= "table,th,td{";
      s+= "border: 1px solid black;";
      s+= "border-collapse: collapse;";
      s+= "}";
      s+= "th,td{";
      s+= "padding: 5px";
      s+= "}";
      s+= "th {";
      s+= "text-align: left;";
      s+= "}";
      s+= "</style>";
      s += "<p ALIGN=RIGHT>Team_Name: ";
      for (int i = 0; i < 32; ++i) {
        t+=  char(EEPROM.read(32 + i));
      }
      Serial.println(t);
      int a= t.length();
      Serial.println(a);
      for (int i = 0; i < a; ++i) {
       s+=  char(EEPROM.read(32 + i));
      }
      //s+=char(EEPROM.read(32 + 0));
      //s+=char(EEPROM.read(32 + 1));
      s+= "</p>";
      s+= "<table style='width:100%'>";
      s+= "<caption>Result</caption>";
      s+= "<tr>";
      s+= "<th>Resources</th>";
      s+= "<th>Points</th>";
      s+= "<th>No. of resources detected</th>";
      s+= "</tr>";
      s+= "<tr>";
      s+= "<td>Titanium</td>";
      s+= "<td>30</td>";
      s+= "<td>";
      s+= Titanium.Read;
      s+= "</td>";
      s+= "</tr>";

      s+= "<tr>";
      s+= "<td>Gas</td>";
      s+= "<td>7</td>";
      s+= "<td>";
      s+= Gas.Read;
      s+= "</td>";
      s+= "</tr>";

      s+= "<tr>";
      s+= "<td>Water</td>";
      s+= "<td>10</td>";
      s+= "<td>";
      s+= Water.Read;
      s+= "</td>";
      s+= "</tr>";

      s+= "<tr>";
      s+= "<td>Diamond</td>";
      s+= "<td>45</td>";
      s+= "<td>";
      s+= Diamond.Read;
      s+= "</td>";
      s+= "</tr>";
      
      s+= "<tr bgcolor='#CFC9C8'>";
      s+= "<td>FinalScore</td>";
      s+= "<td> </td>";
      s+= "<td>";
      s+= Finalscore_Read;
      s+= "</td>";
      s+= "</tr>";
      s+= "</table>";
      s += "<br><br><p ALIGN=RIGHT><a href=\"/erase_button\">ERASE</a></p>";
      //s.replace("##1122",t);
    WEB_SERVER.send(200, "text/html", makePage("Result of competeion", s));
    });
    WEB_SERVER.on("/erase_button", []() {
        for (int i = 0; i <= 512; ++i) {
        EEPROM.put(i, ascii_space);
        EEPROM.commit();
      }
     String s= "<h4>Memory Erased</h4>";
     s+= "<p>slide switch and press reset for next team to play.</p>";
      WEB_SERVER.send(200, "text/html", makePage("Memory reset", s));
    });
    }
   WEB_SERVER.begin();
  
  }


String makePage(String title, String contents) {
  String s = "<!DOCTYPE html><html><head>";
  s += "<meta name=\"viewport\" content=\"width=device-width,user-scalable=0\">";
  s += "<style>";
  // Simple Reset CSS
  s += "*,*:before,*:after{-webkit-box-sizing:border-box;-moz-box-sizing:border-box;box-sizing:border-box}";
  s += "html{font-size:100%;-ms-text-size-adjust:100%;-webkit-text-size-adjust:100%}html,button,input";
  s += ",select,textarea{font-family:sans-serif}article,aside,details,figcaption,figure,footer,header,";
  s += "hgroup,main,nav,section,summary{display:block}body,form,fieldset,legend,input,select,textarea,";
  s += "button{margin:0}audio,canvas,progress,video{display:inline-block;vertical-align:baseline}";
  s += "audio:not([controls]){display:none;height:0}[hidden],template{display:none}img{border:0}";
  s += "svg:not(:root){overflow:hidden}body{font-family:sans-serif;font-size:16px;font-size:1rem;";
  s += "line-height:22px;line-height:1.375rem;color:#585858;font-weight:400;background:#fff}";
  s += "p{margin:0 0 1em 0}a{color:#cd5c5c;background:transparent;text-decoration:underline}";
  s += "a:active,a:hover{outline:0;text-decoration:none}strong{font-weight:700}em{font-style:italic}";
  // Basic CSS Styles
  s += "body{font-family:sans-serif;font-size:16px;font-size:1rem;line-height:22px;line-height:1.375rem;";
  s += "color:#585858;font-weight:400;background:#fff}p{margin:0 0 1em 0}";
  s += "a{color:#cd5c5c;background:transparent;text-decoration:underline}";
  s += "a:active,a:hover{outline:0;text-decoration:none}strong{font-weight:700}";
  s += "em{font-style:italic}h1{font-size:32px;font-size:2rem;line-height:38px;line-height:2.375rem;";
  s += "margin-top:0.7em;margin-bottom:0.5em;color:#343434;font-weight:400}fieldset,";
  s += "legend{border:0;margin:0;padding:0}legend{font-size:18px;font-size:1.125rem;line-height:24px;line-height:1.5rem;font-weight:700}";
  s += "label,button,input,optgroup,select,textarea{color:inherit;font:inherit;margin:0}input{line-height:normal}";
  s += ".input{width:100%}input[type='text'],input[type='email'],input[type='tel'],input[type='date']";
  s += "{height:36px;padding:0 0.4em}input[type='checkbox'],input[type='radio']{box-sizing:border-box;padding:0}";
  // Custom CSS
  s += "header{width:100%;background-color: #2c3e50;top: 0;min-height:60px;margin-bottom:21px;font-size:15px;color: #fff}.content-body{padding:0 1em 0 1em}header p{font-size: 1.25rem;float: left;position: relative;z-index: 1000;line-height: normal; margin: 15px 0 0 10px}";
  s += "</style>";
  s += "<title>";
  s += title;
  s += "</title></head><body>";
  s += "<header><p>" + DEVICE_TITLE + "</p></header>";
  s += "<div class=\"content-body\">";
  s += "<img src='https://curiositygym.com/wp-content/uploads/2016/08/CG8PC.png' alt='Lamp' width='278' height='100'>";
  s += contents;
  s += "</div>";
  s += "</body></html>";
  return s;
}

String urlDecode(String input) {
  String s = input;
  s.replace("%20", " ");
  s.replace("+", " ");
  s.replace("%21", "!");
  s.replace("%22", "\"");
  s.replace("%23", "#");
  s.replace("%24", "$");
  s.replace("%25", "%");
  s.replace("%26", "&");
  s.replace("%27", "\'");
  s.replace("%28", "(");
  s.replace("%29", ")");
  s.replace("%30", "*");
  s.replace("%31", "+");
  s.replace("%2C", ",");
  s.replace("%2E", ".");
  s.replace("%2F", "/");
  s.replace("%2C", ",");
  s.replace("%3A", ":");
  s.replace("%3A", ";");
  s.replace("%3C", "<");
  s.replace("%3D", "=");
  s.replace("%3E", ">");
  s.replace("%3F", "?");
  s.replace("%40", "@");
  s.replace("%5B", "[");
  s.replace("%5C", "\\");
  s.replace("%5D", "]");
  s.replace("%5E", "^");
  s.replace("%5F", "-");
  s.replace("%60", "`");
  return s;
}

int readBlock(int blockNumber, byte arrayAddress[]) 
{
  int largestModulo4Number=blockNumber/4*4;
  int trailerBlock=largestModulo4Number+3;
  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
         Serial.print("PCD_Authenticate() failed (read): ");
         return 3;
  }     
  byte buffersize = 18;
  status = mfrc522.MIFARE_Read(blockNumber, arrayAddress, &buffersize);
  if (status != MFRC522::STATUS_OK) {
          Serial.print("MIFARE_read() failed: ");    
          return 4;
  }
}
