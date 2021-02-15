/* ULU.07 Universal 4-bit 7-segment display - program code */ 
CC-BY-SA Jeroen Brinkman */ 

int value, setting, pointer, look, todo; 
int show1[16][4] = 
{{0,0,0,0},{0,0,0,1},{0,0,1,0},{0,0,1,1},{0,1,0,0},{0,1,0,1},{0,1,1,0},{0,1,1,1},{1,0,0,0},{1,0,0,1},{1,0,1,0},{1,0,1,1},{1,1,0,0},{1,1,0,1},{1,1,1,0},{1,1,1,1}};
int show2[8][7] = 
{{1,1,1,1,1,1,0},{0,1,1,0,0,0,0},{1,1,0,1,1,0,1},{1,1,1,1,0,0,1},{0,1,1,0,0,1,1},{1,0,1,1,0,1,1},{1,0,1,1,1,1,1},{1,1,1,0,0,0,0}};
int show3[8][5][11] = {  // values for Lb, Lc, Le, Lg, Ra, Rb, Rc, Rd, Re, Rf, Rg
{{0,0,0,0,1,1,1,1,1,1,1}, {0,0,0,0,1,1,1,1,1,1,1}, {0,0,0,1,1,1,1,1,1,1,0}, {0,0,0,1,1,1,1,0,0,0,0}, {0,0,0,1,1,1,1,1,1,1,1}},
{{0,0,0,0,1,1,1,1,0,1,1}, {0,0,0,0,1,1,1,1,0,1,1}, {0,0,0,1,0,1,1,0,0,0,0}, {0,0,0,1,1,0,1,1,1,1,1}, {0,0,0,1,1,1,1,0,0,0,0}},
{{0,0,0,0,1,1,1,0,1,1,1}, {1,1,0,0,1,1,1,1,1,1,0}, {0,0,0,1,1,1,0,1,1,0,1}, {0,0,0,1,1,0,1,1,0,1,1}, {0,0,0,1,1,0,1,1,1,1,1}},
{{0,0,0,0,0,0,1,1,1,1,1}, {1,1,0,0,0,1,1,0,0,0,0}, {0,0,0,1,1,1,1,1,0,0,1}, {0,0,0,1,0,1,1,0,0,1,1}, {0,0,0,1,1,0,1,1,0,1,1}},
{{0,0,0,0,1,0,0,1,1,1,0}, {1,1,0,0,1,1,0,1,1,0,1}, {0,0,0,1,0,1,1,0,0,1,1}, {0,0,0,1,1,1,1,1,0,0,1}, {0,0,0,1,0,1,1,0,0,1,1}},
{{0,0,0,0,0,1,1,1,1,0,1}, {1,1,0,0,1,1,1,1,0,0,1}, {0,0,0,1,1,0,1,1,0,1,1}, {0,0,0,1,1,1,0,1,1,0,1}, {0,0,0,1,1,1,1,1,0,0,1}},
{{0,0,0,0,1,0,0,1,1,1,1}, {1,1,0,0,0,1,1,0,0,1,1}, {0,0,0,1,1,0,1,1,1,1,1}, {0,0,0,1,0,1,1,0,0,0,0}, {0,0,0,1,1,1,0,1,1,0,1}},
{{0,0,0,0,1,0,0,0,1,1,1}, {1,1,0,0,1,0,1,1,0,1,1}, {0,0,0,1,1,1,1,0,0,0,0}, {0,0,0,0,1,1,1,1,1,1,0}, {0,0,0,1,0,1,1,0,0,0,0}}
};
#define LB 10
#define LC 8
#define LE 11
#define LG 12
#define LP 7
#define RA 0
#define RB 1
#define RC 3
#define RD 5
#define RE 6
#define RF 4
#define RG 9
#define RP 2
#define B0 A3
#define B1 A2
#define B2 A1
#define B3 A0
#define SW1D A4
#define SW2U A5
#define SW2D A6
#define SW3U A7

void setup() {
  // declare the ledPins as an OUTPUT:
   pinMode(LB, OUTPUT); // left display, segment  b
   pinMode(LC, OUTPUT); // left display, segment  c
   pinMode(LE, OUTPUT); // left display, segment  e
   pinMode(LG, OUTPUT); // left display, segment  g
   pinMode(LP, OUTPUT); // left display, segment  dp
   pinMode(RA, OUTPUT); // right display, segment  a
   pinMode(RB, OUTPUT); // right display, segment  b
   pinMode(RC, OUTPUT); // right display, segment  c
   pinMode(RD, OUTPUT); // right display, segment  d
   pinMode(RE, OUTPUT); // right display, segment  e
   pinMode(RF, OUTPUT); // right display, segment  f
   pinMode(RG, OUTPUT); // right display, segment  g
   pinMode(RP, OUTPUT); // right display, segment  dp
   pinMode(13, OUTPUT); // blinking led showing the program hartbeat
   pinMode(B3, INPUT); // Bit 3
   pinMode(B2, INPUT); // Bit 2
   pinMode(B1, INPUT); // Bit 1
   pinMode(B0, INPUT); // Bit 0
   pinMode(SW1D, INPUT_PULLUP); 
   pinMode(SW2U, INPUT_PULLUP); 
   pinMode(SW2D, INPUT_PULLUP); 
   pinMode(SW3U, INPUT); 
   /* Every ULU with an Arduino introduces itself. This one uses the 7-segment display */ 
   digitalWrite(RA, HIGH); digitalWrite(RB, HIGH);  digitalWrite(RC, HIGH); delay(1000);
   digitalWrite(RA, LOW); digitalWrite(RB, LOW);  digitalWrite(RC, LOW); delay(1000);
};

int readvalue() {
   int v0, v1, v2, v3;
   v0 = analogRead(B0) > 500 ? 1 :  0;
   v1 = analogRead(B1) > 500 ? 1 :  0;
   v2 = analogRead(B2) > 500 ? 1 :  0;
   v3 = analogRead(B3) > 500 ? 1 :  0;
   return v0 + (v1 * 2) + (v2 * 4) + (v3 * 8);
}

int readsetting() {
   int r, s1d, s2u, s2d, s3u;
   s1d = analogRead(SW1D) < 500 ? 1 : 0; // Switch 1 is on, switch uses pull down resistor so equals 0 when on
   s2u = analogRead(SW2U) < 500 ? 1 : 0; // Switch 2 is up, switch uses pull down resistor so equals 0 when on
   s2d = analogRead(SW2D) < 500 ? 1 : 0; // switch 2 is down, switch uses pull down resistor so equals 0 when on
   s3u = analogRead(SW3U) < 500 ? 0 : 1; // display is on, switch puts +5V to connector
   r = 2;
   if (s2u == 1) {
      r = 1;
   } else { 
      if (s2d == 1) r = 3;
   }
   if (s1d == 1) r = r + 3;
   if (s3u == 0) r = 0; // if display needs to be off, result = 0
   if ((s2u==1)&&(s2d==1)) r = 7; // if testmodus result = 7
   return r;
   // 0. Display off
   // 1. Binary
   // 2. Hexdecimal
   // 3. Decimal
   // 4. Signed
   // 5. 1-complement
   // 6. 2-complement
   // 7. System test
}

void loop(){
   digitalWrite(13, (millis() / 1000) % 2); //1s hartbeat for the onbord led
   setting = readsetting();
   if (setting == 7){
      delay(500);
      value = value + 1; setting = 2;
      if (value == 17) value = 0; // reset the counter
   } else {
      value = readvalue();
   }
   todo = 4; 
   if (value < 8) todo = 2;
   if (value == 16) todo = 3; // all segments on
   if (setting == 0) todo = 0; // dimming the display
   if (setting == 1) todo = 1; // show binary
   // todo = 0 -> display off
   // todo = 1 -> binary display
   // todo = 2 -> value less than 8, so only write data to right display
   // todo = 3 -> all segments on
   // todo = 3 -> value > 7 display two segments
   digitalWrite(LP, (setting == 6) ? HIGH : LOW); // write the left dot if 2-complement
   digitalWrite(RP, setting > 4 ? HIGH : LOW); // write the right dot if 1-complement or 2-complement
   switch (todo) {
   case 0: // dim display
      digitalWrite(LB, LOW);
      digitalWrite(LC, LOW);
      digitalWrite(LE, LOW);
      digitalWrite(LG, LOW);
      digitalWrite(LP, LOW);
      digitalWrite(RA, LOW);
      digitalWrite(RB, LOW);
      digitalWrite(RC, LOW);
      digitalWrite(RD, LOW);
      digitalWrite(RE, LOW);
      digitalWrite(RF, LOW);
      digitalWrite(RG, LOW);
      digitalWrite(RP, LOW);
      break;
   case 1: // binary
      digitalWrite(LB, LOW);
      digitalWrite(LG, LOW);
      digitalWrite(LE, (show1[value][0] == 1 ) ? HIGH : LOW);
      digitalWrite(LC, (show1[value][1] == 1 ) ? HIGH : LOW);
      digitalWrite(RA, LOW);
      digitalWrite(RB, LOW);
      digitalWrite(RD, LOW);
      digitalWrite(RF, LOW);
      digitalWrite(RG, LOW);
      digitalWrite(RE, (show1[value][2] == 1 ) ? HIGH : LOW);
      digitalWrite(RC, (show1[value][3] == 1 ) ? HIGH : LOW);
      break;
   case 2: // value < 8 // only write data to right display
      digitalWrite(LB, LOW);
      digitalWrite(LG, LOW);
      digitalWrite(LE, LOW);
      digitalWrite(LC, LOW);
      digitalWrite(RA, (show2[value][0] == 1 ) ? HIGH : LOW);
      digitalWrite(RB, (show2[value][1] == 1 ) ? HIGH : LOW);
      digitalWrite(RC, (show2[value][2] == 1 ) ? HIGH : LOW);
      digitalWrite(RD, (show2[value][3] == 1 ) ? HIGH : LOW);
      digitalWrite(RE, (show2[value][4] == 1 ) ? HIGH : LOW);
      digitalWrite(RF, (show2[value][5] == 1 ) ? HIGH : LOW);
      digitalWrite(RG, (show2[value][6] == 1 ) ? HIGH : LOW);
      break;
   case 3: // all segments on
      digitalWrite(LB, HIGH);
      digitalWrite(LC, HIGH);
      digitalWrite(LE, HIGH);
      digitalWrite(LG, HIGH);
      digitalWrite(LP, HIGH);
      digitalWrite(RA, HIGH);
      digitalWrite(RB, HIGH);
      digitalWrite(RC, HIGH);
      digitalWrite(RD, HIGH);
      digitalWrite(RE, HIGH);
      digitalWrite(RF, HIGH);
      digitalWrite(RG, HIGH);
      digitalWrite(RP, HIGH);
      break;
   case 4: // write data on both displays
       look = value - 8; pointer = setting - 2;
      digitalWrite(LB, (show3[look][pointer][0] == 1 ) ? HIGH : LOW);
      digitalWrite(LC, (show3[look][pointer][1] == 1 ) ? HIGH : LOW);
      digitalWrite(LE, (show3[look][pointer][2] == 1 ) ? HIGH : LOW);
      digitalWrite(LG, (show3[look][pointer][3] == 1 ) ? HIGH : LOW);
      digitalWrite(RA, (show3[look][pointer][4] == 1 ) ? HIGH : LOW);
      digitalWrite(RB, (show3[look][pointer][5] == 1 ) ? HIGH : LOW);
      digitalWrite(RC, (show3[look][pointer][6] == 1 ) ? HIGH : LOW);
      digitalWrite(RD, (show3[look][pointer][7] == 1 ) ? HIGH : LOW);
      digitalWrite(RE, (show3[look][pointer][8] == 1 ) ? HIGH : LOW);
      digitalWrite(RF, (show3[look][pointer][9] == 1 ) ? HIGH : LOW);
      digitalWrite(RG, (show3[look][pointer][10] == 1 ) ? HIGH : LOW);
    }
}
