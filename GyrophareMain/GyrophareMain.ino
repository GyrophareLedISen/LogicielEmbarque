/*
 * Programme Gyrophare ISEN
 * 
 * Benoit PIRIOU & Rudja RULLE
 * 
 * Decembre 2016 - Fevrier 2017
 */

//--Mode d'execution--//

#define TEST_MODE 0
#define CLASSIC_MODE 1


#include "EclairageBleu.h"
//#include "Bluetooth.h"

#include <SoftwareSerial.h>

//Include for ledMatrix
#include <SPI.h>
#include "bitBangedSPI.h"
#include "MAX7219_Dot_Matrix.h"

//Initialization des ports pour la communication bluetooth
SoftwareSerial mySerial(7, 8); // RX, TX

//Initialisation de l'écran d'affichage myDisplay(nombre de max, LOAD, DIN, CLK)
MAX7219_Dot_Matrix myDisplay (4, 12, 13, 11);

int TailleTexte = 20;
int avanceText = 0;

int ledToSwitchOn = 1; 
int maxLedToSwitchOn = 2048;

//Gestion de la machine d'état
long previousMillisText = 0;
long previousMillisGyro = 0;

//Ici on définit le temps qu'il faut attendre avant de faire avancer d'un cran le programme
//Exemple il faut ici qu'il y ai eu 25 ms avant d'allumer la prochaine rangée de LED
long intervalText = 100;
long intervalGyro = 25;

const char * text = "";
String textToShow = "";
String inString = "";

//Setup for SIRENE
unsigned int frequency = 40; //An example. The min frequency on UNO is 31!
unsigned long durationTone = 1000; //Also an example

void setup() {
        Serial.begin(57600);
        while (!Serial) {
          ; // wait for serial port to connect. Needed for native USB port only
        }
        //--SetupTextToShow--//
        myDisplay.begin ();
        myDisplay.sendSmooth ("Hello ISEN",0);
          
        //--SetupEclairage--//
        pinMode(2, OUTPUT); //latchPin
        pinMode(3, OUTPUT); //ClockPin
        pinMode(4, OUTPUT); //DataPin
        pinMode(16,OUTPUT); //16
        pinMode(17,OUTPUT); //1
        pinMode(18,OUTPUT); //18
        pinMode(19,OUTPUT); //19

        //--SetupButton--//
        pinMode(9, INPUT); //BUTTON2. Je ne me souviens plus de l'entrée donc j'ai mis 9 en attendant

        // set the data rate for the SoftwareSerial port
        mySerial.begin(9600);
        mySerial.println("Hello, world?");
}

void loop() {

  if (!mySerial.available()) {
          //Initialisation 
          unsigned long currentMillis = millis();
          
          if (currentMillis - previousMillisGyro > intervalGyro) {
            previousMillisGyro = currentMillis;
            //APPEL DU PROGRAMME GYRO
            if (ledToSwitchOn <= maxLedToSwitchOn)
            {
              AnimationBleu(1,0, ledToSwitchOn);
              ledToSwitchOn = ledToSwitchOn *2;
            } else {
              ledToSwitchOn = 1;
            }
          }else if (currentMillis - previousMillisText > intervalText) {
            previousMillisText = currentMillis;
            if (avanceText<TailleTexte){
              
              // Length (with one extra character for the null terminator)
              int str_len = textToShow.length(); 

              //Rewrite of TailleText to have all the String on the LCD
              //*********************TO DO****************************
              
              // Prepare the character array (the buffer) 
              char char_array[str_len];

              //Convert string into charArray
              textToShow.toCharArray(char_array, str_len);
              
              myDisplay.sendSmooth (char_array,avanceText);
              avanceText++;
            }else {
              avanceText=0;
            }
          }else if (digitalRead(9)){
              //Si le bouton de la sirene est allumé
              //A tester!!!!!
              //If duration is specified, we don't need to call a "noTone" function. 
              //tone(9,frequency,durationTone);
              tone(9,frequency);
          } else if (!digitalRead(9)) {
              //Si le bouton de la sirene est éteint
              noTone(9);
          }

  } else {
          char inChar = mySerial.read();
          inString += inChar;     

          //Apparemment le inChar == '\n' ne fonctionne pas! 
          //Je compare donc par rapport aux valeurs décimales des symboles
          //correspondant à la fin de ligne (cf https://fr.wikipedia.org/wiki/Fin_de_ligne)
          if (inChar == 13 || inChar == 10) {
            Serial.print("String:");
            Serial.print(inString);
            Serial.println("EndString");

            //Desfois le module bluetooth sauvegarde une chaine de caractère vide
            //Il faut dans ce cas retaper la chaine jusqu'à ce que cette dernière 
            //soit prise en compte
            //Avec la fonction ci-dessus, je vais vérifier si la chaine est vide 
            //et décider de l'enregistrer ou pas. 
            
            /*if (inString != '\0' || inString != '\n') {
              Serial.println("WRITE");
              textToShow = inString;
            }*/

            //Gestion de la longueur de la chaine à afficher.
            //En fonction de la longueur de la chaine, je modifie la variable 
            //"TailleTexte". Cette dernière détermine le nombre de fois que l'écran
            //doit avancer d'un cran pour afficher l'ensemble de la chaine. 
            int str_len = textToShow.length(); 
            TailleTexte = (str_len * 6);
            
            textToShow = inString;
            // clear the string for new input:
            inString = "";
          }
  } 
}
