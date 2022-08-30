// connecting x-box

#include<XBOXONESBT.h>
#include<usbhub.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include<SPI.h>

USB Usb;
BTD Btd(&Usb);

XBOXONESBT Xbox(&Btd,PAIR);

//Motor Driver
#define EnR 2 // RIGHT WHEEL
#define R_AN1 3 // ANTI
#define R_CL2 4 // CLOCK

#define EnL 5 //LEFT WHEEL 
#define L_AN3 6 // ANTI
#define L_CL4 7 // CLOCK

//defined Speed
#define Speeda 53
#define Speedb 53

//Triggered Controlled Speed
int speedLT;
//speedLT = Xbox.getButtonPress(LT)/4;

 void setup()
{
//X-box  
  Serial.begin(115200);
 #if !defined(__MIPSEL__)
  while (!Serial);
 #endif
  if(Usb.Init() == -1){
    Serial.println("\r\n OSC did not start");
    while(1);
  }
  Serial.println("\r\n Xbox One S bluetooth Library Started ");
}


//Wheels
void forward(){
     speedL();
     digitalWrite(R_AN1, LOW);
     digitalWrite(R_CL2, HIGH); 
     digitalWrite(L_AN3, LOW);
     digitalWrite(L_CL4, HIGH); 
     }

void backward(){
     speedL();
     digitalWrite(R_AN1, HIGH);
     digitalWrite(R_CL2, LOW); 
     digitalWrite(L_AN3, HIGH);
     digitalWrite(L_CL4, LOW); 
     }

void stop(){
     analogWrite(EnR,Speeda);
     analogWrite(EnL,Speedb);
     digitalWrite(R_AN1, LOW);
     digitalWrite(R_CL2, LOW); 
     digitalWrite(L_AN3, LOW);
     digitalWrite(L_CL4, LOW);
}

void right(){
     speedL();
     digitalWrite(R_AN1, LOW);
     digitalWrite(R_CL2, HIGH); 
     digitalWrite(L_AN3, HIGH);
     digitalWrite(L_CL4, LOW);
    }

void left(){
     speedL();
     digitalWrite(R_AN1, HIGH);
     digitalWrite(R_CL2, LOW); 
     digitalWrite(L_AN3, LOW);
     digitalWrite(L_CL4, HIGH);
    }

void speedL(){
     analogWrite(EnR,speedLT);
     analogWrite(EnL,speedLT);
    }

  


 void loop()
{
  Usb.Task();

  if( Xbox.connected() )
  {
    
    if( Xbox.getButtonClick(X) )
       Serial.println("X"); // X Shows Y
    if( Xbox.getButtonClick(Y) )
       Serial.println("Y"); // Not showing, when showing through XBOXONESBT printing LB or like that 
    if( Xbox.getButtonClick(UP) )
       Serial.println("UP"); // UP get clicked two times 
       
    if( Xbox.getButtonClick(DOWN) )
       Serial.println("Down");
    if( Xbox.getButtonClick(LEFT) )
       Serial.println("Left");
    if( Xbox.getButtonClick(RIGHT) )
       Serial.println("Right");

    if( Xbox.getButtonClick(VIEW) )
       Serial.println("View");
    if( Xbox.getButtonClick(MENU) )
       Serial.println("Menu");

    if( Xbox.getButtonClick(A) )
    {
       Serial.println("A");
       //Serial.println("Wheels Rotating Clockwise");
       //Serial.println("In Forward Direction for (RIGHT)");
       //forward();
      // right();
    }
    if( Xbox.getButtonClick(B) )
    {
       Serial.println("B");
       Serial.println("Wheels Stop Rotating");
       Serial.println("Stop");
       stop();
    }
    if( Xbox.getButtonClick(LT) ) 
       Serial.println("LT");
    if( Xbox.getButtonClick(RT) )
       Serial.println("RT");

    if( Xbox.getButtonPress(LT)>0 ){
      Serial.println();
      Serial.print("LT: ");
      Serial.print(Xbox.getButtonPress(LT));
      Serial.println();
      //for PWM
      speedLT = Xbox.getButtonPress(LT)/4;
      Serial.print("\t");
      Serial.print("speedLT: ");
      Serial.print(speedLT);
      //speedL called
      speedL();
      Serial.println();
      }
    if( Xbox.getButtonPress(RT)>0 ) {
      Serial.println();
      Serial.print("RT: "); 
      Serial.print(Xbox.getButtonPress(RT)); 
      Serial.println();
    }

    if(Xbox.getAnalogHat(RightHatX) == -32768 ){
      Serial.println();
      Serial.println("Left Side Extreme");
      Serial.print("RightHatX:");
      Serial.print(Xbox.getAnalogHat(RightHatX));
      Serial.println("Left Turn");
     left();
    }

    if(Xbox.getAnalogHat(RightHatX) == 32767 ){
      Serial.println();
      Serial.println("Right Side Extreme");
      Serial.print("RightHatX:");
      Serial.print(Xbox.getAnalogHat(RightHatX));
      Serial.println("Right Turn");
      right();
    }

    if(Xbox.getAnalogHat(LeftHatY) == -32768 ){
      Serial.println();
      Serial.println("Up Side Extreme");
      Serial.print("LeftHatY:");
      Serial.print(Xbox.getAnalogHat(LeftHatY));
      Serial.println("Forward");
      forward();
    }

     if(Xbox.getAnalogHat(LeftHatY) == 32767 ){
      Serial.println();
      Serial.println("Down Side Extreme");
      Serial.print("LeftHatY:");
      Serial.print(Xbox.getAnalogHat(LeftHatY));
      Serial.println("backward");
      backward();
    }

   static uint16_t vibLT, vibRT;
    if (Xbox.getButtonPress(LT) != vibLT || Xbox.getButtonPress(RT) != vibRT) {
      vibLT = Xbox.getButtonPress(LT);
      vibRT = Xbox.getButtonPress(RT);
      uint8_t leftRumble = map(vibLT, 0, 1023, 0, 255); // Map the trigger values into a byte
      uint8_t rightRumble = map(vibRT, 0, 1023, 0, 255);
      if (leftRumble > 0 || rightRumble > 0)
        Xbox.setRumbleOn(leftRumble, rightRumble, leftRumble, rightRumble);
      else
        Xbox.setRumbleOff();
    }
       
     
    }
  }
