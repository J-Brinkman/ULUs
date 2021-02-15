/* ULU.08 Adder & ALU - program code */ 
/* CC BY-NC-SA Jeroen Brinkman */ 

int i, signa, action, l, r, o, t; // integers
int rc, l0, l1, l2, l3, r0, r1, r2, r3, o0, o1, o2, o3, oc; // bits
bool latchmode, lastlatch, latch, show, twocomp; // booleans

const int conv[24][4] = {{1,0,0,0}, {1,0,0,1}, {1,0,1,0}, {1,0,1,1}, {1,1,0,0}, {1,1,0,1}, {1,1,1,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,1}, {0,0,1,0}, {0,0,1,1}, {0,1,0,0}, {0,1,0,1}, {0,1,1,0}, {0,1,1,1}, {1,0,0,0}, {1,0,0,1}, {1,0,1,0}, {1,0,1,1}, {1,1,0,0}, {1,1,0,1}, {1,1,1,0}, {1,1,1,1}}; // array

#define BOUNCE 8

#define OVER 1
#define R1 2
#define R2 3
#define R3 4
#define L0 5
#define L1 6
#define L2 7
#define L3 8
#define AC0 9
#define AC1 10
#define AC2 11
#define AC3 12
#define HEART 13
#define O3 A0
#define O2 A1
#define O1 A2
#define O0 A3
#define TWOCO A4
#define R0 A5
#define LATCH A6
#define RC A7

void setup() {
   pinMode(RC, INPUT);
   pinMode(L0, INPUT);
   pinMode(L1, INPUT);
   pinMode(L2, INPUT);
   pinMode(L3, INPUT);
   pinMode(R0, INPUT);
   pinMode(R1, INPUT);
   pinMode(R2, INPUT);
   pinMode(R3, INPUT);
   pinMode(O0, OUTPUT);
   pinMode(O1, OUTPUT);
   pinMode(O2, OUTPUT);
   pinMode(O3, OUTPUT);
   pinMode(HEART,  OUTPUT); // blinking led showing the programma's hartbeat
   pinMode(OVER, OUTPUT);
   pinMode(AC0, INPUT);
   pinMode(AC1, INPUT);
   pinMode(AC2, INPUT);
   pinMode(AC3, INPUT);
   pinMode(LATCH, INPUT);
   pinMode(TWOCO, INPUT_PULLUP);

   /* Initialize variables */ 
   lastlatch = false; latchmode = false; o3 = 0; o2 = 0; o1 = 0; o0 = 0; oc = 0;

   /* Every ULU with an Arduino introduces itself. This one uses a pulse count */ 
   digitalWrite(O3, HIGH); delay (100); digitalWrite(O3, LOW); delay (200);
   digitalWrite(O3, HIGH); delay (100); digitalWrite(O3, LOW); delay (200);
   digitalWrite(O3, HIGH); delay (100); digitalWrite(O3, LOW); delay (200);
   digitalWrite(O3, HIGH); delay (100); digitalWrite(O3, LOW); delay (200);
   digitalWrite(O3, HIGH); delay (100); digitalWrite(O3, LOW); delay (200);
   digitalWrite(O3, HIGH); delay (100); digitalWrite(O3, LOW); delay (200);
   digitalWrite(O3, HIGH); delay (100); digitalWrite(O3, LOW); delay (200);
   digitalWrite(O3, HIGH); delay (100); digitalWrite(O3, LOW); delay (200);
}

void loop(){
   digitalWrite(HEART, (millis() / 1000) % 2); //1s heartbeat for the onboard led

  /* Read all input */ 
   signa = 0;
   for (i = 0; i < BOUNCE; i++){signa += (analogRead(LATCH) > 500 ? 1 : 0); delay(1);}  // eliminate bouncing
   latch = (signa > (i / 2));

   action = ((digitalRead(AC0) == HIGH ? 1 : 0) + (digitalRead(AC1) == HIGH ? 2 : 0) + (digitalRead(AC2) == HIGH ? 4 : 0) + (digitalRead(AC3) == HIGH ? 8 : 0));  
   twocomp = (digitalRead(TWOCO) == LOW);

   l0 = digitalRead(L0); l1 = digitalRead(L1); l2 = digitalRead(L2); l3 = digitalRead(L3);
   r0 = digitalRead(R0);  r1 = digitalRead(R1); r2 = digitalRead(R2);  r3 = digitalRead(R3); rc = (analogRead(RC) > 500 ? 1 : 0);
   l =  l0 + (l1 * 2) + (l2 * 4) + (l3 * 8);  
   r =  r0 + (r1 * 2) + (r2 * 4) + (r3 * 8);
   if (twocomp && (action != 7)){
      if (l > 7) l = l - 16;
      if (r > 7) r = r - 16;
   }

   /* Write current output */ 
   show = false; latchmode = latchmode || latch;
   if (!latchmode) show = true;
   if (!latch && lastlatch) show = true;
   lastlatch = latch;

   if (show){
      digitalWrite(OVER, (oc == 1 ) ? HIGH : LOW);
      digitalWrite(O3, (o3 == 1 ) ? HIGH : LOW);
      digitalWrite(O2, (o2 == 1 ) ? HIGH : LOW);
      digitalWrite(O1, (o1 == 1 ) ? HIGH : LOW);
      digitalWrite(O0, (o0 == 1 ) ? HIGH : LOW);
   }

/* Process input and start the ALU */ 
   o = 99; oc = 0; 
   switch (action) {
      case 0: // r + l + rc
         o = l + r + rc;
         break;
      case 1: // l
         o = r;
         break;
      case 2: // l=r+c,  l<r+c,  l>r+c,  l=0
         o3 = (l = r + rc) ? 0 : 1; o2 = (l < r + rc) ? 0 : 1; o1 = (l > r + rc) ? 0 : 1; o0 = (l = 0) ? 0 : 1;
         break;
      case 3: // l - 1
         o = l - 1;
         break;
      case 4: // r + c - l
         o = r + rc - l;
         break;
      case 5: // l AND r
         o3 = l3 && r3; o2 = l2 && r2; o1 = l1 && r1; o0 = l0 && r0; 
         break;
      case 6: // shift left r, c
         o3 = l2; o2 = l1; o1 = l0; o0 = 0;
         break;
      case 7: // l to 2-comp
        // has already been show
         break;
      case 8: // OR
         o3 = l3 || r3; o2 = l2 || r2; o1 = l1 || r1; o0 = l0 || r0; 
         break;
      case 9: // XOR
         o3 = l3 ^ r3; o2 = l2 ^ r2; o1 = l1 ^ r1; o0 = l0 ^ r0; 
         break;
      case 10: // Not l
          o3 = (l3 == 1 ? 0 : 1); o2 = (l2 == 1 ? 0 : 1); o1 = (l1 == 1 ? 0 : 1); o0 = (l0 == 1 ? 0 : 1);
         break;
      case 11: // shift r right
         o3 = 0; o2 = l3; o1 = l2; o0 = l1;
         break;
      case 12: // l + 1
         o = l + 1;
         break;
      case 13: // rotate r left lx When l is zero, rotate just once
         o3 = l2; o2 = l1; o1 = l0; o0 = l3;  
         for (i = 0; i < r - 1; i++){
            t = o2; o2 = o1; o1 = o0; o0 = o3; o3 = t;
         }
         break;
      case 14: // l * (r + c)
         o = l * (r + rc);
         break;
      case 15: // rotate right a
        o = l - r;
         break;
   }
   if (o != 99){
      if (twocomp){
         if (o > 7) {
            o = 0; oc = 1;
         }
         if (o < -8) {
            o = 0; oc = 1;
         }
      } else {
         if (o < 0) {
            o = 0; oc = 1;
         }
         if (o > 15) {
            o = o - 16; oc = 1;
         }
      }
      o0 = conv[o+8][3]; o1 = conv[o+8][2]; o2 = conv[o+8][1]; o3 = conv[o+8][0]; 
   }
}
