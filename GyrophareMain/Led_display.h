


int Led_Display_Programme(int avanceText, String textToShow, MAX7219_Dot_Matrix *myDisplay){
      MAX7219_Dot_Matrix * newDisplay;
      newDisplay = myDisplay;
      //delay(50);
      // Length (with one extra character for the null terminator)
      int str_len = textToShow.length(); 

      //Rewrite of TailleText to have all the String on the LCD
      //*********************TO DO****************************
      
      // Prepare the character array (the buffer) 
      char char_array[str_len];
      //Serial.println("Le texte a afficher est");
      //Serial.println(textToShow);
      //delay(50);
      
      //Convert string into charArray
      textToShow.toCharArray(char_array, str_len);
      
      //NE FONCTIONNE PAS!!! JE ne sais pas pk
      newDisplay->sendSmooth(char_array, avanceText);
      //delay(50);
      avanceText++;
      return avanceText;
}

