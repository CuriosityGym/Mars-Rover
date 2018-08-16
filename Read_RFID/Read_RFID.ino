#include <SPI.h>
#include <EEPROM.h>
#include <MFRC522.h>
#define SS_PIN D8 
#define RST_PIN D3  
MFRC522 mfrc522(SS_PIN, RST_PIN);       
MFRC522::MIFARE_Key key;
int block=38;
byte readbackblock[18];
int integer_variable,Finalscore;
struct Resource {
  int count;
  int address;
  int score;
  int point;
};
struct Resource Water; 
struct Resource Gas;
struct Resource Titanium;
struct Resource Diamond;

void setup() {
Water.count = 0;
Water.address = 0;
Water.score;
Water.point=10;

Gas.count = 0;
Gas.address = 1;
Gas.score;
Gas.point=7;

Titanium.count = 0;
Titanium.address = 2;
Titanium.score;
Titanium.point=30;

Diamond.count = 0;
Diamond.address = 3;
Diamond.score;
Diamond.point=45;

EEPROM.begin(64);
Serial.begin(9600);       
SPI.begin();               
mfrc522.PCD_Init();       
Serial.println("Scan a MIFARE Classic card");
for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
}

void loop()
{
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
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
  EEPROM.get(Water.address,integer_variable);
  Serial.println(integer_variable);
    }
   else if(myString == "Gas"){
    Gas.count++;
  EEPROM.put(Gas.address,Gas.count); 
  EEPROM.commit();
  Serial.print("EEPROM contents at Gas Address is  : ");
  EEPROM.get(Gas.address,integer_variable);
  Serial.println(integer_variable);
    }
  else if(myString == "Titanium"){
    Titanium.count++;
  EEPROM.put(Titanium.address,Titanium.count); 
  EEPROM.commit();
  Serial.print("EEPROM contents at Titanium Address is  : ");
  EEPROM.get(Titanium.address,integer_variable);
  Serial.println(integer_variable);
    }
  else if(myString == "Diamond"){
    Diamond.count++;
  EEPROM.put(Diamond.address,Diamond.count); 
  EEPROM.commit();
  Serial.print("EEPROM contents at Diamond Address is  : ");
  EEPROM.get(Diamond.address,integer_variable);
  Serial.println(integer_variable);
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

