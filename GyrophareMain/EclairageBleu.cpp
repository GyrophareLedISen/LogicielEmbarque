/* 
 * File:   EclairageBleu.cpp
 * Author: BenoitP
 * 
 * Created on 7 december 2016, 23:54
 */
 
#include "EclairageBleu.h"
#include "Arduino.h"

void AnimationBleu(int num_Animation, int compt_Animation, int ledToSwitchOn)
{ 
  //Les autres PIN.
  switch(num_Animation)
  {
    case 0 : //INIT
        { 
          GyroDisplay(4095);
          delay(200);
          GyroDisplay(0);
          delay(200);         
          break;
        }
    case 1 : //annimation de 1 a 8
        {
          GyroDisplay(ledToSwitchOn);
          //delay(50);
          /*GyroDisplay(2);
          delay(50);
          GyroDisplay(4);
          delay(50);
          GyroDisplay(8);
          delay(50);
          GyroDisplay(16);
          delay(50);
          GyroDisplay(32);
          delay(50);
          GyroDisplay(64);
          delay(50);
          GyroDisplay(128);
          delay(50);
          GyroDisplay(256);
          delay(50);
          GyroDisplay(512);
          delay(50);
          GyroDisplay(1024);
          delay(50);
          GyroDisplay(2048);
          delay(50);  */     
          break;
        }        
                  
    default : //animation par defaut , on ne fait rien
        {
          break;
        }
  }
}

void GyroDisplay(int numberToDisplay)
{
  int latchPin=2;
  int clockPin=3;
  int dataPin=4;
  // 16 17 18 19

  /* -
   * 1 
   * 2 
   * -
   * 4
   * 8
   * -
   * 16
   * 32
   * -
   * 64
   * 128
   * -
   * 256
   * 1024
   * -
   * 2048
   * 4096
   * -
   */

    if( (numberToDisplay-2047)>0) {digitalWrite(19,HIGH); numberToDisplay=numberToDisplay-2048;} else {digitalWrite(19,LOW);}
    if( (numberToDisplay-1023)>0) {digitalWrite(18,HIGH); numberToDisplay=numberToDisplay-1024;} else {digitalWrite(18,LOW);}
    if( (numberToDisplay-511)>0) {digitalWrite(17,HIGH); numberToDisplay=numberToDisplay-512;} else {digitalWrite(17,LOW);}
    if( (numberToDisplay-255)>0) {digitalWrite(16,HIGH); numberToDisplay=numberToDisplay-256;} else {digitalWrite(16,LOW);} 

   //Traitement de numberToDisplay
    
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);  

    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    // pause before next value:


    //delay(500);
}

