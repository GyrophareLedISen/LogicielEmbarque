//Sirene variables
//Tone for the police
unsigned int tone1 = 435; //An example. The min frequency on UNO is 31!
unsigned int tone2 = 580; //An example. The min frequency on UNO is 31!
unsigned long durationTone = 1000; //Also an example

bool Sirene_Program (bool stateSirene, const int Sirene_pin){
      if (stateSirene == true){
        //We switch on the first tone of the
        tone(Sirene_pin,tone1);
      } else {
        //We switch to an another tone
        tone(Sirene_pin,tone2);
      }
      return !stateSirene;
}

