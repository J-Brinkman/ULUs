/* ULU.06  Dual JK flip-flop - program code  */
/* CC BY-NC-SA Jeroen Brinkman */ 

bool triggerleft, triggerright; // booleans
int i;
int jleft, lastjleft, cleft, lastcleft, kleft, lastkleft, setleft, qleft, switchleft; // left variables
int jright, lastjright, cright, lastcright, kright, lastkright, setright, qright, switchright; // right variables

#define BOUNCE 8
#define WAIT 150
#define RJ 2
#define RC 3
#define RK 4
#define RU 5
#define RQ 6
#define LJ 7
#define LC 8
#define LK 9
#define LU 10
#define LQ 11
#define LD A0
#define RD A1
#define HEART  13

void setup() {
   /* Define I/O pins */ 
   pinMode(RJ,  INPUT); 
   pinMode(RC,  INPUT); 
   pinMode(RK,  INPUT); 
   pinMode(RU,  INPUT_PULLUP); 
   pinMode(RQ,  OUTPUT); 
   pinMode(LJ,  INPUT); 
   pinMode(LC,  INPUT); 
   pinMode(LK,  INPUT); 
   pinMode(LD,  INPUT_PULLUP); 
   pinMode(LQ,  OUTPUT); 
   pinMode(HEART, OUTPUT); // blinking led showing the program hartbeat
   pinMode(RD,  INPUT); 
   pinMode(LD,  INPUT); 

   /* Every ULU with an Arduino introduces itself. This one uses blinking leds */ 
   digitalWrite(LQ, HIGH); delay(WAIT); 
   digitalWrite(LQ, LOW); digitalWrite(RQ, HIGH); delay(WAIT);
   digitalWrite(LQ, HIGH); digitalWrite(RQ, LOW); delay(WAIT);
   digitalWrite(LQ, LOW); digitalWrite(RQ, HIGH); delay(WAIT);
   digitalWrite(LQ, HIGH); digitalWrite(RQ, LOW); delay(WAIT);
   digitalWrite(LQ, LOW); digitalWrite(RQ, HIGH); delay(WAIT); digitalWrite(RQ, LOW); 

   /* Initialize variables */ 
   triggerleft = false; setleft = 0; qleft = 0;
   lastcleft = 0; lastjleft = 0; lastkleft = 0;
   triggerright = false; setright = 0; qright = 0;
   lastcright = 0; lastjright = 0; lastkright = 0;
}

void loop(){
   digitalWrite(HEART, (millis() / 1000) % 2); //1s hartbeat for the onbord led

   /* read the switches */
   switchright =  !digitalRead(RU) + (!digitalRead(RD) * 2);
   switchleft =  !digitalRead(LU) + (!digitalRead(LD) * 2);

   /* read the input */
   jleft = 0; kleft = 0; cleft = 0;
   jright = 0; kright = 0; cright = 0;
   for (i = 0; i < BOUNCE; i++) { //debounce routine
      jright += digitalRead(RJ);
      cright += digitalRead(RC); 
      kright += digitalRead(RK);
      jleft += digitalRead(LJ);
      cleft += digitalRead(LC);
      kleft += digitalRead(LK);
      delay(1);
   }
   jright = (jright > i / 2) ? 1 : 0;
   cright = (cright > i / 2) ? 1 : 0;
   kright = (kright > i / 2) ? 1 : 0;
   jleft = (jleft > i / 2) ? 1 : 0;
   cleft = (cleft > i / 2) ? 1 : 0;
   kleft = (kleft > i / 2) ? 1 : 0;

   /* Determine right trigger */
   triggerright = ((cright = 0) && (lastcright == 1)) ? true : false;
   if ((switchright == 0) &&  (lastjright == 1) && (jright = 0)) triggerright = true;
   if ((switchright == 0) &&  (lastkright == 1) && (kright = 0)) triggerright = true;
   lastjright = jright; lastcright = cright; lastkright = kright; 

   /* Determine right action */
   if (switchright == 0) {
         setright = 0;
         if (jright == 1) setright = 1;
         if (cright == 1) setright = 2;
         if (kright == 1) setright = 3;
   } else {
         if (cright == 1) setright = (jright * 2) + kright;
   }

   /* determine & write right output */
   if (triggerright ){
      triggerright = false;
      switch (setright) {
         case 0: // NOP
            break;
         case 1: // reset
            qright = 0;
            break;
         case 2: // set
            qright = 1;
            break;
         case 3: // toggle
            qright = (qright == 0) ? 1 : 0;
         break;
      }
   digitalWrite(RQ, (qright == 1 ) ? HIGH : LOW);
   }

   /* Determine left trigger */
   triggerleft = ((cleft = 0) && (lastcleft == 1)) ? true : false;
   if ((switchleft == 0) &&  (lastjleft == 1) && (jleft = 0)) triggerleft = true;
   if ((switchleft == 0) &&  (lastkleft == 1) && (kleft = 0)) triggerleft = true;
   lastjleft = jleft; lastcleft = cleft; lastkleft = kleft; 

   /* determine left action */
   if (switchleft == 0) {
         setleft = 0;
         if (jleft == 1) setleft = 1;
         if (cleft == 1) setleft = 2;
         if (kleft == 1) setleft = 3;
   } else {
         if (cleft == 1) setleft = (jleft * 2) + kleft;
   }

   /* determine & write left output */
   if (triggerleft ){
      triggerleft = false;
      switch (setleft) {
         case 0: // NOP
            break;
         case 1: // reset
            qleft = 0;
            break;
         case 2: // set
            qleft = 1;
            break;
         case 3: // toggle
            qleft = (qleft == 0) ? 1 : 0;
         break;
      }
   digitalWrite(RQ, (qleft == 1 ) ? HIGH : LOW);
   }
}
