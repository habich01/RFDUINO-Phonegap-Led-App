//This is a script that combines examples "connect" and "led button".
//This will have the led be Red when its bluetooth is not connected 
//and have its led be green when connected. When the android button
//is pushed, the led will be blue

//*********NOTES************////
//added String inString
//onRecieve, added   
//inString += data[i];
//int value = inString.toInt();
//value == 1 controls blue led 

//Need bluetooth code
#include <RFduinoBLE.h>

//instantiate led pins
int advertisement_led = 2; //red
int connection_led    = 3; //greem
int app_led           = 4; //blue


//instantiate button pins
int buttonA = 5;
int buttonB = 6;

//instantiate program variables
int debounce_time    = 10;
int debounce_timeout = 100;
String inString      = "";
bool BLEoff          = true;
int button           = buttonA; //default


void setup() {
  // put your setup code here, to run once:
  RFduinoBLE.deviceName = "CH - 1";
  RFduinoBLE.advertisementData = "LEDex";
  
  pinMode(advertisement_led, OUTPUT);
  pinMode(connection_led, OUTPUT);
  pinMode(app_led, OUTPUT);
  pinMode(buttonA, INPUT);
  pinMode(buttonB, INPUT);
  
  //start the BLE stack
  //RFduinoBLE.begin();
  turnLed(false);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //button press = up and down = high and low cycle 
  delay_until_button(HIGH);
  delay_until_button(LOW);
}

/////////*************instantiate program functions***********////////////////

//debounce the button to confrim the read out
int debounce(int state) {
  
  //if the state is high, determine which button was pressed
  //if state low, use button that was pressed
  if(state){
    if(RFduino_pinWoke(buttonA))
      button = buttonA;
    else
      button = buttonB;
  }
  
  int start = millis();
  int debounce_start = start;
  
  while (millis() - start < debounce_timeout){
    
    if(digitalRead(button) == state){
      
      if(millis() - debounce_start >= debounce_time){
        return 1;
      }
      
    }
    else{
      debounce_start = millis();
    }
  }   
  
  return 0;
}

//put processor into almost sleep mode until button A is pressed
int delay_until_button(int state){
  //set button edge to wake up on
  if(state){
      RFduino_pinWake(buttonA, HIGH);
      RFduino_pinWake(buttonB, HIGH);
    }else{
      RFduino_pinWake(button, LOW); // pinWake low can't do more than 1, therefore use
                                    // generic button var and adjust
  }
  
  
  do
    //switch to lower power until a button edge wakes up
    RFduino_ULPDelay(INFINITE);
  while(! debounce(state));
  
  //if multiple buttons were configured put code in here
  if(RFduino_pinWoke(buttonA) || RFduino_pinWoke(buttonB)){
    
    //When its button B
    if(button == buttonB && BLEoff && state){
      BLEoff = false;
      turnLed(true);
      RFduinoBLE.begin();
    }else if(button == buttonB && !BLEoff && state) {
      BLEoff = true;
      RFduinoBLE.end();
      turnLed(false);
    }
    
    //When button A
    if(button == buttonA && state){
      RFduinoBLE.sendFloat(1);
    }else if(button == buttonA && !state) {
      RFduinoBLE.sendFloat(0);
    }
    
    RFduino_resetPinWake(buttonA);
    RFduino_resetPinWake(buttonB);
    
  }

}

int turnLed(bool on) {
  
  if(on){
    digitalWrite(advertisement_led, LOW);
    digitalWrite(connection_led, LOW);
    digitalWrite(app_led, LOW);
  }else{
    digitalWrite(advertisement_led, HIGH);
    digitalWrite(connection_led, HIGH);
    digitalWrite(app_led, HIGH);
  }  

}

///////**********This section is for Rfduino BLE code*********/////////////
  
//when bluetooth begins to broadcast
void RFduinoBLE_onAdvertisement(bool start)
{

  // turn the red led on if we start advertisement, and turn it
  // off if we stop advertisement
  if (start)
    digitalWrite(advertisement_led, HIGH);
  else
    digitalWrite(advertisement_led, LOW);
}

//when bluetooth connects
void RFduinoBLE_onConnect()
{
  digitalWrite(connection_led, HIGH);
}

//when bluetooth disconnectes
void RFduinoBLE_onDisconnect()
{
  // don't leave the appl or connection led on 
  digitalWrite(app_led, LOW);
  digitalWrite(connection_led, LOW);
}

//when bluetooth recieves data
void RFduinoBLE_onReceive(char *data, int len)
{

  //Take incoming data and build to string
  for(int i=0; i<len ; i++){
    inString += data[i];
  }
  //convert value to number
  int value = inString.toInt();
  
  // based on number do following
  if (value == 1){
    digitalWrite(app_led, HIGH);
    digitalWrite(connection_led, LOW);
  }
  else{
    digitalWrite(app_led, LOW);
    digitalWrite(connection_led, HIGH);
  }
  
  
  //Do before leaving function
  
  inString = ""; //reset the string
}


