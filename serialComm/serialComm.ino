#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11 ,12);

String input = "";
bool sendDim = false;

void setup()
{
	lcd.begin(20, 4);
	lcd.setCursor(0, 0);
//	lcd.print("Hello, ");

	Serial.begin(9600);
	//pinMode(13, OUTPUT);
//	Serial.println("Available");
	
  /* add setup code here */
	
}

void loop()
{
	if (Serial.available() > 0){
    String input = Serial.readString();
    if (input == "#") lcd.clear();
    else if (input == "g") sendDim = true;
    else lcd.print(input);
	}

 else if (sendDim){
  Serial.print("20X4");
  Serial.println();
  sendDim = false;
 }
	
}
