#include <SoftwareSerial.h>


String bluetoothText (String stringVar) {
        SoftwareSerial mavoieserie(7,8); // pin TX RX du module Bluetooth 
        
        char c;
        String string = stringVar;
        String etape;

        //On a choisi de prendre des int, car le programme n'accepte pas les booleans
        int stringComplete = 0;
        int etapeComplete = 0;

        
        mavoieserie.begin(9600);
        //mavoieserie.println("AT+NAMEGyrophare Isen"); // nom du module bluetooth 
        mavoieserie.println(" Choisir votre état : 1, 2 ,3 ou 4 suivi de /");
        mavoieserie.println(mavoieserie.available());
if (mavoieserie.available()) {
                              
                c = mavoieserie.read(); 
                mavoieserie.println("ICI");
                string += c;  // concaténation des octets reçus                              
                if(c== '/') {
                  stringComplete = true;
  
                  if (stringComplete) // chose à faire quand la chaine de caractère est complète 
                  {

                  // écrire l'action à faire ici 
                  //serial.println(string);
                  return string;
                  
                    string = "";
                    stringComplete = false;
                    //etape = "";
                    //etapeComplete = false;
                    mavoieserie.println(" Choisir votre état : 1, 2 ,3 ou 4 suivi de /"); //retour au choix de l'étape 
                  } 
       } 
       return string;
}
    
}
          
