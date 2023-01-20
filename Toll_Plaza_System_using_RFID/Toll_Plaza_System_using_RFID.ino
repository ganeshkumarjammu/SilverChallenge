/***********************************************************
Project Title : Toll Plaza System
Organisation: Soham Academy Of Human Excellence
Website: https://ria.sohamacademy.org/
***********************************************************/

#include <LiquidCrystal_I2C.h> // Library for LCD
#include <Wire.h> // Library for I2C communication
#include <SPI.h>
#include "MFRC522.h"
#include <stdio.h>
#include <Servo.h>
Servo tollGateServo;   
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
#define SS_PIN 10
#define RST_PIN 9
const boolean DEBUG = true;
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
const int GREENLED = 8; //D8 is Connected to GreenLED
const int REDLED = 7;  //D7 is Connected to RedLED
const int btnListPin = A0;
int INITIALAMOUNT=200;
int BALANCEAMOUNT=200;
int AMOUNT2DEDUCT=50;
//Structure for users
const int iConstantNumOfUsers = 1;
int iCard = 0 ; 
String newCardID = "" ;

struct UsersStruct
{
  String sRFIDCode[iConstantNumOfUsers]; 
  String sName[iConstantNumOfUsers];
  int iInitialAmount[iConstantNumOfUsers];
  int iBalanceAmount[iConstantNumOfUsers];
  int iDeductedAmount[iConstantNumOfUsers];
  int iStatus[iConstantNumOfUsers];
} structUsers;

// SET UIDs 
String sRFIDCodes[iConstantNumOfUsers] = {""};

// Names 
String sUserNames[iConstantNumOfUsers] = {"  Name: RiA"}; //SchoolID

void printDebugMsg(String aStrMessage)
{
    if (DEBUG)
    {
      Serial.println(aStrMessage); 
    }
}
void loadUsersToStruct()
{
  // Start loading the data into structUsers
  for(int i=0; i<iConstantNumOfUsers; i++)
  {
    structUsers.sRFIDCode[i] = sRFIDCodes[i];
    structUsers.sName[i] = sUserNames[i];
    structUsers.iInitialAmount[i] = INITIALAMOUNT;
    structUsers.iBalanceAmount[i] = BALANCEAMOUNT;//IA-DA
    structUsers.iDeductedAmount[i]= AMOUNT2DEDUCT;
    structUsers.iStatus[i] = 0;//OUT
    //printDebugMsg(" Adding " + structUsers.sRFIDCode[i] + " " + structUsers.sName[i] + " " + structUsers.iInitialAmount[i] + " " + structUsers.iBalanceAmount[i]+" " + structUsers.iDeductedAmount[i]);
  }
  //displayAllData();
}

boolean isValidRFIDCode(String aStrRFIDCode)
{
  for(int i=0; i<iConstantNumOfUsers; i++)
  {
    if (sRFIDCodes[i] == aStrRFIDCode) return true;
    else if(newCardID == aStrRFIDCode) return true;
  }
  Serial.print("RFID Code:") ;
  Serial.println(aStrRFIDCode);
  return false;
}

void displayAllData()
{
  String sLine1 = "", sLine2 = "";
  for(int i=0; i<iConstantNumOfUsers; i++)
  {
    sLine1 = structUsers.sName[i];
    sLine2 = "I:" + String(structUsers.iInitialAmount[i]) + " B:" + String(structUsers.iBalanceAmount[i]) + " D:" + String(structUsers.iDeductedAmount[i]);;
    printDebugMsg("Displaying.. ");
    printDebugMsg(sLine1);
    printDebugMsg(sLine2);
    displayOnScreen(sLine1, sLine2);
    delay(1000);
  }
}

void displayOnScreen(String asLine1, String asLine2)
{
    printDebugMsg("Displaying On Screen...");
    lcd.clear();
    lcd.setCursor(0, 0);
    printDebugMsg(asLine1);
    lcd.print(asLine1);
    lcd.setCursor(0, 1);
    printDebugMsg(asLine2);
    lcd.print(asLine2);
}

void displayWelcomeMessage()
{
    printDebugMsg("Displaying Welcome Message...");
    String sLine1 = "  Welcome to";
    String sLine2 = "Soham Toll Plaza";
    displayOnScreen(sLine1, sLine2);
}

void alertLED()
{
    digitalWrite(REDLED, HIGH);
    delay(1000);
    digitalWrite(REDLED, LOW);
}

void allowPassage()
{
    tollGateServo.write(120);
    digitalWrite(GREENLED, HIGH);
    delay(2000);
    tollGateServo.write(0);
    digitalWrite(GREENLED, LOW);
}

void processCard(String aStrID)
{
  String sLine1 = "";
  String sLine2 = "";
  
  if (!isValidRFIDCode(aStrID)) 
  {
    if( iCard == 0 ){
      newCardID = aStrID;
      iCard = 1; 
    }else{
    sLine1 = "Invalid Card";
    sLine2 = aStrID;
    alertLED();
    displayOnScreen(sLine1, sLine2);
    delay(1000);
    displayWelcomeMessage();
    return;
    }
  }
  
  // Get the location of the user in the userStruct.
  int iUserInStructUsers = -1;
  for(int i=0; i<iConstantNumOfUsers; i++)
  {
    if (sRFIDCodes[i] == aStrID) 
    {
      iUserInStructUsers = i;
    }
    else if(newCardID == aStrID){
      iUserInStructUsers = 0;
    }
  }
  //printDebugMsg("Updated User: RFID = " + structUsers.sRFIDCode[iUserInStructUsers]+ "Name = " + structUsers.sName[iUserInStructUsers] + " INITIAL AMT = " + String(structUsers.iInitialAmount[iUserInStructUsers]) + "BALANCE AMT = " + String(structUsers.iBalanceAmount[iUserInStructUsers]));
  
  // Show message in the LCD about the updated user
  sLine1 = structUsers.sName[iUserInStructUsers];
  // Check if there is enough balance
  if (structUsers.iBalanceAmount[iUserInStructUsers] < AMOUNT2DEDUCT)
  {
    sLine2 = "!!Recharge Now!!";
    displayOnScreen(sLine1, sLine2);
    alertLED();
  }
  else
  {
     // Deduct the amount
    structUsers.iBalanceAmount[iUserInStructUsers] = structUsers.iBalanceAmount[iUserInStructUsers] - AMOUNT2DEDUCT;
    sLine2 = "I:" + String(structUsers.iInitialAmount[iUserInStructUsers]) + " B:" + String(structUsers.iBalanceAmount[iUserInStructUsers]) + " D:" + String(structUsers.iDeductedAmount[iUserInStructUsers]); 
    printDebugMsg("Line 1: " + sLine1);
    printDebugMsg("Line 2: " + sLine2);
    displayOnScreen(sLine1, sLine2);
    delay(1000);
    allowPassage();
    delay(1000);
    displayWelcomeMessage();
  }
}

void setup()
{
  Serial.begin(9600);
  lcd.begin();
  SPI.begin();
  rfid.PCD_Init();
  tollGateServo.attach(5);    // Tollgate servo                       
  tollGateServo.write(0);                             
  delay(200);                     
  pinMode(btnListPin, INPUT);
  pinMode(GREENLED, OUTPUT);
  pinMode(REDLED, OUTPUT);
  digitalWrite(GREENLED, LOW);
  digitalWrite(btnListPin, HIGH);
  // Record all the available IDs to structUsers
  loadUsersToStruct();
  displayWelcomeMessage();
}

void loop()
{
  if (digitalRead(btnListPin) == LOW)
  {
    displayAllData();
    delay(1000);
    displayWelcomeMessage();
  }
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if ( ! rfid.PICC_ReadCardSerial())
    return;
  
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K)
  {
    Serial.println(F("Your tag is not of type Soham Classic, your card/tag can't be read :("));
  }
  String StrID = "" ;
  for (byte i = 0; i < 4; i ++)
  {
    StrID +=
    (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
    String(rfid.uid.uidByte[i], HEX) +
    (i != 3 ? ":" : "" );
  }
  StrID.toUpperCase();
  Serial.print("RFID CARD ID:");
  Serial.println(StrID);
  processCard(StrID);
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  Serial.println("--Soham RiA --");
}
