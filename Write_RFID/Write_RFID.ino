#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN D8  
#define RST_PIN D3  
MFRC522 mfrc522(SS_PIN, RST_PIN);        
MFRC522::MIFARE_Key key;
int block=38;
byte blockcontent[16] = {"Titanium"};
//byte blockcontent[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void setup() {
     Serial.begin(9600);        
     SPI.begin();              
     mfrc522.PCD_Init();        
     Serial.println("Scan a MIFARE Classic card");
     for (byte i = 0; i < 6; i++) {
          key.keyByte[i] = 0xFF;//keyByte is defined in the "MIFARE_Key" 'struct' definition in the .h file of the library
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
  Serial.println("card selected");
  writeBlock(block, blockcontent);//the blockcontent array is written into the card block
}

int writeBlock(int blockNumber, byte arrayAddress[]) 
{
  int largestModulo4Number=blockNumber/4*4;
  int trailerBlock=largestModulo4Number+3;
  if (blockNumber > 2 && (blockNumber+1)%4 == 0){Serial.print(blockNumber);Serial.println(" is a trailer block:");return 2;}
  Serial.print(blockNumber);
  Serial.println(" is a data block:");
  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
         Serial.print("PCD_Authenticate() failed: ");
         //Serial.println(mfrc522.GetStatusCodeName(status));
         return 3;
  }
status = mfrc522.MIFARE_Write(blockNumber, arrayAddress, 16);
  
  if (status != MFRC522::STATUS_OK) {
           Serial.print("MIFARE_Write() failed: ");
           //Serial.println(mfrc522.GetStatusCodeName(status));
           return 4;
  }
  Serial.println("block was written");
}

