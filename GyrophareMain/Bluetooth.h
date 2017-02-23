
String Bluetooth_Program(String textToShow, SoftwareSerial *mySerial) {
      SoftwareSerial * serial = mySerial;
      serial = mySerial;
      String inString = "";
      Serial.println("Bluetooth_Program");
      while(serial->available() > 0)
      {
          char inChar = serial->read();
          inString += (char)inChar;
          Serial.println(inString);   

          //Apparemment le inChar == '\n' ne fonctionne pas! 
          //Je compare donc par rapport aux valeurs décimales des symboles
          //correspondant à la fin de ligne (cf https://fr.wikipedia.org/wiki/Fin_de_ligne)
          if (inChar == 13 || inChar == 10 ){//|| inChar == '\n') {
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
            
            textToShow = inString;
            // clear the string for new input:
            inString = "";
          }
      } 
      return textToShow;
}
