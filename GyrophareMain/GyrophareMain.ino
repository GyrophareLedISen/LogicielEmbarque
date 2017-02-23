#include <SoftwareSerial.h>
#include "bitBangedSPI.h"
#include "MAX7219_Dot_Matrix.h"
#include "Gyro.h"
#include "Led_display.h"
#include "Sirene.h"
#include "Bluetooth.h"

const int rx = 8;
const int tx = 7;
SoftwareSerial mySerial(rx,tx); // RX, TX

// Déclarer les états possibles et la variable qui va mémoriser l'état courant
enum { 
  GYRO, 
  LED_DISPLAY, 
  SIRENE,
  DEFAULT_MODE,
  BLUETOOTH,
  OTHER 
  } State = GYRO;
// GYRO : on allume une rangée de LED sur le gyrophare
// LED : on fait avancer d'un cran le texte sur la matrice à LED
// SIRENE : on fais fonctionner la sirène
// DEFAULT_MODE : mode de configuration par défault
// OTHER : autre mode en cas de besoin

//************ALL GLOBAL VARIABLES******************
//Led display variables
const String defaultText = "Police\n"; //OK
bool defaultMode = true;
int TailleTexte = 20;
int avanceText = 0;
String textToShow = "";
const int numberMax = 4;
const int load = 12;
const int din = 13;
const int clk = 11;
//Initialisation de l'écran d'affichage myDisplay(nombre de max, LOAD, DIN, CLK)
MAX7219_Dot_Matrix myDisplay (numberMax, load, din, clk);

//Sirene variables
const int button_On_Off_Sirene = 21;
int sensorReading;
int stateButtonSirene;
bool stateSirene = false;
const int Sirene_pin = 9;

//Gyro variables
const int button_Animation = 20;
//To check how many time we pressed the animation button
int stateButtonAnimation;
int lastButtonStateAnimation = 0;
int buttonPushCounter = 0;   // counter for the number of button presses

//Animation variables
int animation1[] = {1,2,4,8,16,32,64,128,256,512,1024,2048};
int animation2[] = {63,4095,4032,4095};
int animation3[] = {3591,0,504,0};
int animation4[] = {4095,2046,1020,504,240,96,240,504,1020,2046,4095,3999,3855,3591,3075,2049,0};

//Time variables
long previousMillisText = 0;
long previousMillisGyro = 0;
long previousMillisSirene = 0;
long intervalText = 100; //anciennement : 100
long intervalGyro = 25;
long intervalSirene = 1000;

//ALL CONSTANTS VARIABLES
const int ledSirene = 6;
const int ledDefault = 5;
const int cmd_delay = 50;

//Temporary variable
int incrementAnim1 = 0;
int incrementAnim2 = 0;

void setup()
{
        Serial.begin(57600);
        while (!Serial) {
            ; // wait for serial port to connect. Needed for native USB port only
        }
        //--SetupLedDisplay--//
        myDisplay.begin ();
        for (int i = 0; i < 25; i++) {
          //Je suis obligé de mettre ce mot en majuscule pour que l'initialisation
          //se fasse correctement! Je ne sais pas pourquoi...
          myDisplay.sendSmooth("POLICE", i);
        }
        myDisplay.end();
        myDisplay.begin();

        //--SetupEclairage--//
        pinMode(2, OUTPUT); //latchPin
        pinMode(3, OUTPUT); //ClockPin
        pinMode(4, OUTPUT); //DataPin
        pinMode(16,OUTPUT); //16 A2 OK      
        pinMode(17,OUTPUT); //17 A3 OK
        pinMode(18,OUTPUT); //18 A4 OK
        pinMode(19,OUTPUT); //19 A5 OK

        //--SetupButton--//
        pinMode(button_On_Off_Sirene, INPUT); //BUTTON2. Je ne me souviens plus de l'entrée donc j'ai mis 9 en attendant
        pinMode(button_Animation, INPUT); //BUTTON1.
        
        //--SetupLED--//
        pinMode(ledSirene,OUTPUT); //Led associate to button2
        
        // set the data rate for the SoftwareSerial port
        //Don't change the rate!!!!
        //mySerial.begin(115200);
        mySerial.begin(115200);
        State = GYRO;
        Serial.println("Gyrophare ISEN : INITIALISATION OK");
}

//Remarques
//Lorsque l'arduino est alimenté uniquement par le générateur, 
//la partie bluetooth ne fonctionne plus. 
//Il faut que j'alimente obligatoirement la maquette avec le générateur ET mon ordinateur
//pour que le programme puisse tourner plus de 2 minutes!!
 
void loop()
{
  unsigned long currentMillis = millis();

  //Read sirene button
  sensorReading = analogRead(button_On_Off_Sirene);
  stateButtonSirene = map(sensorReading, 0, 950, 0, 1);

  //Read button animation (button1)
  sensorReading = analogRead(button_Animation);
  stateButtonAnimation = map(sensorReading, 0, 950, 0, 1);

  //Calcul length of default text
  TailleTexte = calculation_Length_of_String(defaultText);
  
  //Decision state
  if (!mySerial.available()) 
  {
    
  } else { //mySerial.available() = 1
     State = BLUETOOTH;
  }
  
  switch (State)
  {
    case GYRO:
      //Action
      //Test timer si test timer ==1 je fais gyro 
      if (currentMillis - previousMillisGyro > intervalGyro) {
        previousMillisGyro = currentMillis;
        //Serial.println("Gyro_STATE");
        if (stateButtonAnimation == 1){
        //if (buttonChoiceAnimation()){  
          if (incrementAnim1 <= (sizeof(animation1)/sizeof(int))){
            AnimationBleu(1,0,animation1[incrementAnim1]);
            incrementAnim1++;
          } else {
            incrementAnim1 = 0;
          }        
        } else { 
          //stateButtonAnimation == 0
          if (incrementAnim2 <= (sizeof(animation2)/sizeof(int))){
            AnimationBleu(1,0,animation2[incrementAnim2]);
            incrementAnim2++;
          } else {
            incrementAnim2 = 0;
          }         
        }
      }
      State = LED_DISPLAY;
      break;
    case LED_DISPLAY:
      //Action
      if (currentMillis - previousMillisText > intervalText) {
        previousMillisText = currentMillis;
        //Serial.println("Led_Display_STATE");
        if (avanceText < TailleTexte){
          if (defaultMode == true){
            digitalWrite(ledDefault, HIGH);
            avanceText = Led_Display_Programme(avanceText,defaultText,&myDisplay);
          } else {
            digitalWrite(ledDefault, LOW);
            avanceText = Led_Display_Programme(avanceText,textToShow,&myDisplay);
          } 
        } else {
            avanceText=0;
        }   
      }
      State = SIRENE;
      break;
    case SIRENE:
      //Action
      if ((currentMillis - previousMillisSirene > intervalSirene) && (stateButtonSirene == 0)) {
        previousMillisSirene = currentMillis; 
        digitalWrite(ledSirene, HIGH);
        Serial.println("Sirene_STATE");
        if (stateSirene == false){
          stateSirene = Sirene_Program(stateSirene, Sirene_pin);
        } else {
          stateSirene = Sirene_Program(stateSirene, Sirene_pin);
        }
      }
      if (stateButtonSirene == 1){
        digitalWrite(ledSirene, LOW);
        noTone(Sirene_pin);
      }
      State = GYRO;
      break;
    case BLUETOOTH:
      //Action
      Serial.println("Bluetooth_STATE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
      defaultMode = false;
      textToShow =  Bluetooth_Program(textToShow, &mySerial);
      
      //Update of TailleTexte variable
      TailleTexte = calculation_Length_of_String(textToShow);
      State = GYRO;
      break;
    case OTHER:
      //Action
      //You can add an other function here! 
      break;
    }  
}

//delay_Function in case of
boolean delay_Function () {
    delay(cmd_delay);
    return true;
}

int calculation_Length_of_String (String text) {
    //Gestion de la longueur de la chaine à afficher.
    //En fonction de la longueur de la chaine, je modifie la variable 
    //"TailleTexte". Cette dernière détermine le nombre de fois que l'écran
    //doit avancer d'un cran pour afficher l'ensemble de la chaine. 
    int str_len = text.length(); 
    return (str_len * 6);
}

/*bool buttonChoiceAnimation(){
    //Read button animation (button1)
  sensorReading = analogRead(button_Animation);
  stateButtonAnimation = map(sensorReading, 0, 950, 0, 1);

    if (stateButtonAnimation != lastButtonStateAnimation) {
      // if the state has changed, increment the counter
      if (stateButtonAnimation == 1) {
        // if the current state is HIGH then the button
        // wend from off to on:
        buttonPushCounter++;
        //Serial.println("on");
        //Serial.print("number of button pushes:  ");
        //Serial.println(buttonPushCounter);
      } else {
        // if the current state is LOW then the button
        // wend from on to off:
        //Serial.println("off");
      }
      // Delay a little bit to avoid bouncing
      //delay(50);
    }
    // save the current state as the last state,
    //for next time through the loop
    lastButtonStateAnimation = stateButtonAnimation;

    // turns on the LED every four button pushes by
    // checking the modulo of the button push counter.
    // the modulo function gives you the remainder of
    // the division of two numbers:
    if (buttonPushCounter % 2 == 0) {
      return true;
    } else {
      return false;
    }
}*/


