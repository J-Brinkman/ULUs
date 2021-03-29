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
   pinMode(RU,  INPUT_PULLUP); 
   pinMode(RD,  INPUT_PULLUP); 
   pinMode(RJ,  INPUT); 
   pinMode(RC,  INPUT); 
   pinMode(RK,  INPUT); 
   pinMode(RQ,  OUTPUT); 
   pinMode(LU,  INPUT_PULLUP); 
   pinMode(LD,  INPUT_PULLUP); 
   pinMode(LJ,  INPUT); 
   pinMode(LC,  INPUT); 
   pinMode(LK,  INPUT); 
   pinMode(LQ,  OUTPUT); 
   pinMode(HEART, OUTPUT); // blinking led showing the program hartbeat

   /* Every ULU with an Arduino introduces itself. This one uses blinking leds */ 
   digitalWrite(LQ, HIGH); delay(WAIT); 
   digitalWrite(LQ, LOW); digitalWrite(RQ, HIGH); delay(WAIT);
   digitalWrite(LQ, HIGH); digitalWrite(RQ, LOW); delay(WAIT);
   digitalWrite(LQ, LOW); digitalWrite(RQ, HIGH); delay(WAIT);
   digitalWrite(LQ, HIGH); digitalWrite(RQ, LOW); delay(WAIT);
   digitalWrite(LQ, LOW); digitalWrite(RQ, HIGH); delay(WAIT); digitalWrite(RQ, LOW); 

   /* Initialize variables */ 
   triggerright = false; setright = 0; qright = 0;
   lastcright = 0; lastjright = 0; lastkright = 0;
   triggerleft = false; setleft = 0; qleft = 0;
   lastcleft = 0; lastjleft = 0; lastkleft = 0;
}

void loop(){
   digitalWrite(HEART, (millis() / 1000) % 2); //1s hartbeat for the onbord led

   /* read the switches */
   switchright =  !digitalRead(RU) + (!digitalRead(RD) * 2);
   switchleft =  !digitalRead(LU) + (!digitalRead(LD) * 2);

   /* read the input */
   jright = 0; cright = 0; kright = 0;
   jleft = 0; cleft = 0; kleft = 0;
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
   triggerright = ((cright == 0) && (lastcright == 1));
   if ((switchright == 0) &&  (lastjright == 1) && (jright == 0)) triggerright = true;
   if ((switchright == 0) &&  (lastkright == 1) && (kright == 0)) triggerright = true;

   /* Determine & execute right action */
   if (triggerright ){
      triggerright = false;
      switch (switchright) {
         case 0: // Single socket input
            if (lastjright == 1) qright = 1;
            if (lastcright == 1) qright = 0;
            if (lastkright == 1) qright = ((qright == 0) ? 1 : 0);
            break;
         case 1: // Store J
            qright =  lastjright;
            break;
         case 2: // Regular use
            if ((lastjright == 1) && (lastkright == 0)) qright = 1;
            if ((lastkright == 1) && (lastjright == 0)) qright = 0;
            if ((lastjright == 1) && (lastkright == 1)) qright = ((qright == 0) ? 1 : 0);
            break;
      }
      digitalWrite(RQ, (qright == 1 ) ? HIGH : LOW);
   }
   lastjright = jright; lastcright = cright; lastkright = kright; 

   /* Determine left trigger */
   triggerleft = ((cleft == 0) && (lastcleft == 1));
   if ((switchleft == 0) &&  (lastjleft == 1) && (jleft == 0)) triggerleft = true;
   if ((switchleft == 0) &&  (lastkleft == 1) && (kleft == 0)) triggerleft = true;

   /* Determine & execute left action */
   if (triggerleft ){
      triggerleft = false;
      switch (switchleft) {
         case 0: // Single socket input
            if (lastjleft == 1) qleft = 1;
            if (lastcleft == 1) qleft = 0;
            if (lastkleft == 1) qleft = ((qleft == 0) ? 1 : 0);
            break;
         case 1: // Store J
            qleft =  lastjleft;
            break;
         case 2: // Regular use
            if ((lastjleft == 1) && (lastkleft == 0)) qleft = 1;
            if ((lastkleft == 1) && (lastjleft == 0)) qleft = 0;
            if ((lastjleft == 1) && (lastkleft == 1)) qleft = ((qleft == 0) ? 1 : 0);
            break;
      }
   digitalWrite(LQ, (qleft == 1 ) ? HIGH : LOW);
   }
   lastjleft = jleft; lastcleft = cleft; lastkleft = kleft; 
}
