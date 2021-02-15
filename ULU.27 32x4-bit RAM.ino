/* ULU.30 27x4-bit RAM memory - program code  */ 
/* CC BY-NC-SA Jeroen Brinkman */ 

int memory[4][16]; // memory
int serad, address, lastaddress, woord; // 4-bit words
int bank, serba, d0, d1, d2, d3; // bits
bool read, write, serial, lastserial; // booleans
int i, display, column, signa, state; // integers

#include "LedControl.h"
LedControl lc=LedControl(12,11,10,2);

#define BOUNCE 8
#define HEART 1
#define DATI0 2
#define DATI1 3
#define DATI2 4
#define DATI3 5
#define AD0 6
#define AD1 7
#define AD2 8
#define AD3 9
#define BANK A0
#define DATO0 A1
#define DATO1 A2
#define DATO2 A3
#define DATO3 A4
#define WRITE  A5
#define READ  A6
#define SERI  A7

void setup() {
   pinMode(HEART, OUTPUT);
   pinMode(DATO0, OUTPUT);
   pinMode(DATO1, OUTPUT);
   pinMode(DATO2, OUTPUT);
   pinMode(DATO3, OUTPUT);
   pinMode(DATI0, INPUT);
   pinMode(DATI1, INPUT);
   pinMode(DATI2, INPUT);
   pinMode(DATI3, INPUT);
   pinMode(BANK, INPUT);
   pinMode(AD0, INPUT);
   pinMode(AD1, INPUT);
   pinMode(AD2, INPUT);
   pinMode(AD3, INPUT);
   pinMode(WRITE, INPUT);
   pinMode(READ, INPUT);
   pinMode(SERI, INPUT);

   /* Initiate dot matrix  */ 
   lc.shutdown(0,false); //The MAX72XX is in power-saving mode on startup, wakeup the display
   lc.shutdown(1,false); //The MAX72XX is in power-saving mode on startup, wakeup the display
   lc.setIntensity(0, 1); // Set the brightness to a low value
   lc.setIntensity(1, 1); // Set the brightness to a low value
   lc.clearDisplay(0); //clear the display
   lc.clearDisplay(1); //clear the display

   /* Every ULU with an Arduino introduces itself. This one uses dot text */ 
   lc.setColumn(0,7,62);  
   lc.setColumn(0,6,32);  
   lc.setColumn(0,5,62);  
   lc.setColumn(0,3,62);  
   lc.setColumn(0,2,32);  
   lc.setColumn(0,1,32);  
   lc.setColumn(1,7,62);  
   lc.setColumn(1,6,32);  
   lc.setColumn(1,5,62);  
   lc.setColumn(1,3,32); 
   delay(800);
   lc.clearDisplay(0); //clear the display
   lc.clearDisplay(1); //clear the display
   lc.setColumn(0,7,58);
   lc.setColumn(0,6,42);
   lc.setColumn(0,5,46);
   lc.setColumn(0,3,2);
   lc.setColumn(0,2,2);
   lc.setColumn(0,1,62);
   lc.setColumn(1,5,50);
   lc.setColumn(1,4,34);
   lc.setColumn(1,3,62);
   lc.setColumn(1,2,42);
   lc.setColumn(1,1,28);
   delay(800);
   lc.clearDisplay(0); //clear the display
   lc.clearDisplay(1); //clear the display
   for (int i = 0; i < 17; i++) {memory[0][i] = 0; memory[1][i] = 0; memory[2][i] = 0; memory[3][i] = 0;} // clear array 
   lastserial = false;  bank = 0; address = 0; lastaddress = 0; state = 0; column = 0;
};

void loop(){
   digitalWrite(HEART, (millis() / 1000) % 2); //1s heartbeat for the onboard led

  /* Read all input  */ 
   signa = 0; 
   for (int i = 0; i < BOUNCE; i++) {signa += (analogRead(SERI) > 500 ? 1 : 0); delay(2);} // eliminate bouncing 
   serial = (signa > i / 2);
   if ((state == 0) && !serial) {
      address =  digitalRead(AD0) + (digitalRead(AD1) * 2) + (digitalRead(AD2) * 4) + (digitalRead(AD3) * 8); 
      bank = (digitalRead(BANK) == HIGH ? 1 : 0);
   }
   d0 =  (digitalRead(DATI0) == HIGH ? 1 : 0); d1 = (digitalRead(DATI1) == HIGH ? 1 : 0); d2 = (digitalRead(DATI2) == HIGH ? 1 : 0); d3 = (digitalRead(DATI3) == HIGH ? 1 : 0); 
   write = (digitalRead(WRITE) == HIGH);
   read =  (analogRead(READ) > 500);
   
   /* Clear output  */ 
   if (!write) {
      digitalWrite(DATO0, LOW);
      digitalWrite(DATO1, LOW);
      digitalWrite(DATO2, LOW);
      digitalWrite(DATO3, LOW);
   }

   /* Manage serial programming  */ 
   // state == 0 -> regular use
   // state == 1 -> Serial programming initiated
   // state == 2 -> Serial programming in progress

   if (serial && state == 0) { // first time serial is 1, serial programming starts
      state = 1; serad = address; serba = bank; // take the actual address as a starting position
   }
   if (state > 0 && (write || read)) state = 0; // when read or write is 1, serial programming ended
   if (serial && !lastserial && (state == 1)) { // only when this condition is met, the memory may be written
      state = 2;
      address = serad; bank = serba;
      write = true;
   }
   lastserial = serial;

   if (address != lastaddress) lc.setColumn(display, column, memory[3][lastaddress]); 
   lastaddress = address;

   /* Write memory  */ 
   // memory[0] - bank 0: - - - - 3 2 1 0
   // memory[1] - bank 1: - - - - 3 2 1 0
   // memory[2] - bank 1: 7 6 5 4 - - - -
   // memory[3] - bank 1 + bank 0: 7 6 5 4 3 2 1 0
   if (write && !read) {
      if (bank == 0) {
         memory[0][address] = d0 + (d1 * 2) + (d2 * 4) + (d3 * 8);
      } else {
         memory[1][address] = d0 + (d1 * 2) + (d2 * 4) + (d3 * 8);
         memory[2][address] = (d0 * 16) + (d1 * 32) + (d2 * 64) + (d3 * 128);
      } 
      memory[3][address] =  memory[2][address] + memory[0][address];
   } 

   /* Post process serial programming  */ 
   if (state == 2) { // serial programming and the memory has been written
      serad += 1;
      if (serad == 16) {
          serad = 0; serba = (serba == 1 ? 0 : 1);
      }
      state = 1;
   }

   /* Read memory  */ 
   if (read && !write) {
      woord = memory[bank][address];
      d3 = 0; d2 = 0; d1 = 0; d0 = 0;
      if (woord > 7) {d3 = 1; woord -= 8;}
      if (woord > 3) {d2 = 1; woord -= 4;}
      if (woord > 1) {d1 = 1; woord -= 2;}
      d0 = woord;
      digitalWrite(DATO0, (d0 == 1 ) ? HIGH : LOW);
      digitalWrite(DATO1, (d1 == 1 ) ? HIGH : LOW);
      digitalWrite(DATO2, (d2 == 1 ) ? HIGH : LOW);
      digitalWrite(DATO3, (d3 == 1 ) ? HIGH : LOW);
   }

   /* Write blinking display  */ 
   // blink bank 0 -> memory[2] and memory[3]
   // blink bank 1 -> memory[0] and memory[3]
   column = address; display = 0;
   if (address < 8) display = 1; else column -= 8;
   if (((millis() / 1000) % 2) == 1) {
      lc.setColumn(display, column, memory[3][address]); // 
   } else {
      if (bank == 0) {
         lc.setColumn(display, column, memory[2][address]); // 
      } else {
         lc.setColumn(display, column, memory[0][address]); // 
      }
   }
}
