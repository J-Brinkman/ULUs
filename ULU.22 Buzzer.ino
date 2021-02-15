/* ULU.22 Buzzer - program code */ 
/* CC BY-NC-SA Jeroen Brinkman */ 

#define BOUNCE 8
int freq, lastfreq; // Frequencies
int note, index, count, maxcount; // Integers
const int tones[16] = {0, 349, 392, 440, 494, 523, 587, 659, 698, 784, 880, 988, 1046, 1175, 1319, 1397};
int input[BOUNCE];

#define ONE  50
#define TWO  100
#define THREE 150
#define SIX  300
#define MORSE 750
#define HEART  1
#define PLAY  2
#define BEEP  3
#define TONE0  4
#define TONE1  5
#define TONE2  6
#define TONE3  7
#define BUZZER  8
#define LED  13

void setup() {
   pinMode(TONE0, INPUT);
   pinMode(TONE1, INPUT);
   pinMode(TONE2, INPUT);
   pinMode(TONE3, INPUT);
   pinMode(BEEP, INPUT);
   pinMode(PLAY, INPUT);
   pinMode(BUZZER, OUTPUT);
   pinMode(LED, OUTPUT);
   pinMode(HEART,  OUTPUT); // blinking led showing the programma's hartbeat

  /* Every ULU with an Arduino introduces itself. This one uses Morse code */ 
   tone(BUZZER, MORSE); delay(ONE); noTone(BUZZER); delay(ONE);
   tone(BUZZER, MORSE); delay(ONE); noTone(BUZZER); delay(ONE);
   tone(BUZZER, MORSE); delay(THREE); noTone(BUZZER); delay(ONE);
   tone(BUZZER, MORSE); delay(THREE); noTone(BUZZER); delay(ONE);
   tone(BUZZER, MORSE); delay(THREE); noTone(BUZZER); delay(ONE);
   delay(TWO);
   tone(BUZZER, MORSE); delay(ONE); noTone(BUZZER); delay(ONE);
   tone(BUZZER, MORSE); delay(ONE); noTone(BUZZER); delay(ONE);
   tone(BUZZER, MORSE); delay(THREE); noTone(BUZZER); delay(ONE);
   tone(BUZZER, MORSE); delay(THREE); noTone(BUZZER); delay(ONE);
   tone(BUZZER, MORSE); delay(THREE); noTone(BUZZER); delay(ONE);
   lastfreq = 0; 
}

void loop(){
   digitalWrite(HEART, (millis() / 1000) % 2); //1s heartbeat for the onboard led

   /* Read note */ 
   for (int i = 0; i < BOUNCE; i++) {
      input[i] =  digitalRead(TONE0) + (digitalRead(TONE1) * 2) + (digitalRead(TONE2) * 4) + (digitalRead(TONE3) * 8);
   }
   /* Determine majority note, in order to avoid problems when a note played longer */ 
   maxcount = 0;
   index = -1;
   for (int i = 0; i < BOUNCE; i++) {
      count = 0;
      for (int j = 0; j < BOUNCE; j++) {
         if (input[i] == input[j]) count++;
      }
      if (count > maxcount) {
         maxcount = count;
         index = i;
      }
   }
   note = input[index];

   /* Determine action */ 
   freq = 0;
   if ((digitalRead(PLAY) == HIGH) && note > 0) freq = tones[note];
   if (digitalRead(BEEP) == HIGH) freq = MORSE;

   /* Play tone */ 
   if ((freq > 0) && (lastfreq !=freq)) {
      tone(BUZZER, freq); 
      digitalWrite(LED, HIGH);
   }
   if ((freq == 0) && (lastfreq > 0)) {
      noTone(BUZZER); 
      digitalWrite(LED, LOW);
   }
   lastfreq = freq;
}
