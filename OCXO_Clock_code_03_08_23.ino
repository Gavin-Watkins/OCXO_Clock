#include <Wire.h>
#include <Adafruit_IS31FL3731.h>
#define TCAADDR 0x77
#define Reset_Pin 2 //reset pin for the demultiplexer
#define Seconds_LED 3 // LEDs between seconds and minutes which can be flashed etc
#define Minutes_LED 4 // LEDs between minutes and hours which can be flashed etc
#define Rotary_switch_pin 12 //switch on rotary encoder for setting time
#define Rotary_count_pin 10 //count input for rotary encoder
#define Rotary_direction_pin 11 //reset pin for the demultiplexer

Adafruit_IS31FL3731_Wing ledmatrix = Adafruit_IS31FL3731_Wing();

// so about to define a big old LUT. y is the horizontal which is 0-6 (7) and x the vertical which is 0-14 (15). 105 values
byte digits[] = {//7x15. Arranged like this so it is obvious what we are doing
  //Start of 0. LUTs take up too much space. Convert to binary and then decode on the other side
  0,1,1,1,1,1,0, // 62
  1,1,0,0,0,0,1, // 97
  1,1,0,0,0,0,1,
  1,1,1,0,0,0,1, // 103
  1,1,1,0,0,0,1,
  1,1,1,1,0,0,1, // 111
  1,1,0,1,0,0,1, // 105
  1,1,0,1,1,0,1, // 109
  1,1,0,0,1,0,1, // 101
  1,1,0,0,1,1,1, // 103
  1,1,0,0,0,1,1, // 99
  1,1,0,0,0,1,1,
  1,1,0,0,0,0,1,
  1,1,0,0,0,0,1,
  0,1,1,1,1,1,0,
  //Start of 1
  0,0,0,1,1,0,0, // 12
  0,0,1,1,1,0,0, // 28
  0,1,1,1,1,0,0, // 60
  0,0,0,1,1,0,0, 
  0,0,0,1,1,0,0,
  0,0,0,1,1,0,0,
  0,0,0,1,1,0,0,
  0,0,0,1,1,0,0,
  0,0,0,1,1,0,0,
  0,0,0,1,1,0,0,
  0,0,0,1,1,0,0,
  0,0,0,1,1,0,0,
  0,0,0,1,1,0,0,
  0,0,0,1,1,0,0,
  0,0,0,1,1,0,0,
  //Start of 2
  0,1,1,1,1,1,0,
  0,0,0,0,0,1,1,
  0,0,0,0,0,1,1,
  0,0,0,0,0,1,1,
  0,0,0,0,0,1,1,
  0,0,0,0,0,1,1,
  0,0,0,0,0,1,1,
  0,1,1,1,1,1,0,
  1,1,0,0,0,0,0,
  1,1,0,0,0,0,0,
  1,1,0,0,0,0,0,
  1,1,0,0,0,0,0,
  1,1,0,0,0,0,0,
  1,1,0,0,0,0,0,
  1,1,1,1,1,1,1,
  //Start of 3
  0,1,1,1,1,1,0,
  0,0,0,0,0,1,1,
  0,0,0,0,0,1,1,
  0,0,0,0,0,1,1,
  0,0,0,0,0,1,1,
  0,0,0,0,0,1,1,
  1,1,1,1,1,1,1,
  0,0,0,0,0,1,1,
  0,0,0,0,0,1,1,
  0,0,0,0,0,1,1,
  0,0,0,0,0,1,1,
  0,0,0,0,0,1,1,
  0,0,0,0,0,1,1,
  0,0,0,0,0,1,1,
  0,1,1,1,1,1,0,
  //Start of 4
  1,1,0,0,0,0,1,
  1,1,0,0,0,0,1,
  1,1,0,0,0,0,1,
  1,1,0,0,0,0,1,
  1,1,0,0,0,0,1,
  1,1,0,0,0,0,1,
  1,1,0,0,0,0,1,
  1,1,0,0,0,0,1,
  1,1,1,1,1,1,1,
  0,0,0,0,0,0,1,
  0,0,0,0,0,0,1,
  0,0,0,0,0,0,1,
  0,0,0,0,0,0,1,
  0,0,0,0,0,0,1,
  0,0,0,0,0,0,1,
  //Start of 5
  1,1,1,1,1,1,0,
  1,1,0,0,0,0,0,
  1,1,0,0,0,0,0,
  1,1,0,0,0,0,0,
  1,1,0,0,0,0,0,
  1,1,0,0,0,0,0,
  0,1,1,1,1,1,0,
  0,0,0,0,0,1,1,
  0,0,0,0,0,1,1,
  0,0,0,0,0,1,1,
  0,0,0,0,0,1,1,
  0,0,0,0,0,1,1,
  0,0,0,0,0,1,1,
  0,0,0,0,0,1,1,
  1,1,1,1,1,1,0,
  //Start of 6
  0,1,1,1,1,1,0,
  1,1,0,0,0,1,1,
  1,1,0,0,0,0,0,
  1,1,0,0,0,0,0,
  1,1,0,0,0,0,0,
  1,1,0,0,0,0,0,
  1,1,0,0,0,0,0,
  1,1,1,1,1,1,0,
  1,1,0,0,0,0,1,
  1,1,0,0,0,0,1,
  1,1,0,0,0,0,1,
  1,1,0,0,0,0,1,
  1,1,0,0,0,0,1,
  1,1,0,0,0,0,1,
  0,1,1,1,1,1,0,
  //Start of 7
  1,1,1,1,1,1,1,
  0,0,0,0,0,1,1,
  0,0,0,0,1,1,0,
  0,0,0,0,1,1,0,
  0,0,0,1,1,0,0,
  0,0,0,1,1,0,0,
  0,0,1,1,0,0,0,
  0,0,1,1,0,0,0,
  0,0,1,1,0,0,0,
  0,0,1,1,0,0,0,
  0,0,1,1,0,0,0,
  0,0,1,1,0,0,0,
  0,0,1,1,0,0,0,
  0,0,1,1,0,0,0,
  0,0,1,1,0,0,0,
  //Start of 8
  0,1,1,1,1,1,0,
  1,1,0,0,0,0,1,
  1,1,0,0,0,0,1,
  1,1,0,0,0,0,1,
  1,1,0,0,0,0,1,
  1,1,0,0,0,0,1,
  0,1,1,1,1,1,0,
  1,1,0,0,0,0,1,
  1,1,0,0,0,0,1,
  1,1,0,0,0,0,1,
  1,1,0,0,0,0,1,
  1,1,0,0,0,0,1,
  1,1,0,0,0,0,1,
  1,1,0,0,0,0,1,
  0,1,1,1,1,1,0,
  //Start of 9
  0,1,1,1,1,1,0,
  1,0,0,0,0,1,1,
  1,0,0,0,0,1,1,
  1,0,0,0,0,1,1,
  1,0,0,0,0,1,1,
  1,0,0,0,0,1,1,
  0,1,1,1,1,1,1,
  0,0,0,0,0,1,1,
  0,0,0,0,0,1,1,
  0,0,0,0,0,1,1,
  0,0,0,0,0,1,1,
  0,0,0,0,0,1,1,
  1,0,0,0,0,1,1,
  1,1,0,0,0,1,1,
  0,1,1,1,1,1,0};

void setCursor(int16_t x0, int16_t y0);
void setTextColor(uint16_t color);
void setTextColor(uint16_t color, uint16_t backgroundcolor);
void setTextSize(uint8_t size);
void setTextWrap(boolean w);

//int sig = 39; // Pin-T1 Pin-39 PD5
byte Count = 0;
boolean toggle2 = 0;
boolean Set_Flag = 0;
byte Set_Counter = 0; //1 is HoursTens, 2 is HoursMinutes, 3 is MinutesTens, 4 is MinutesUnits, 5 is SecondsTens, 6 is SecondsUnits
byte SecondsCount = 0;
byte MinutesCount = 0;
byte HoursCount = 0;
byte SecondsUnits = 0;
byte SecondsTens = 0;
byte MinutesUnits = 0;
byte MinutesTens = 0;
byte HoursUnits = 2;
byte HoursTens = 1;
byte Temp = 0;
byte x = 0;
byte y = 0;
byte brightness = 1;
byte SecondsUnitsSelect = 0;
byte SecondsTensSelect = 0;
byte MinutesUnitsSelect = 0;
byte MinutesTensSelect = 0;
byte HoursUnitsSelect = 0;
byte HoursTensSelect = 0;
int16_t Switch_Sampling = 0;
int Rotary_current = 0;
int Rotary_last = 0;
int Switch_current = 0;
int Switch_last = 0;
byte Switch_position = 0;
long Debouncedelay = 0;
int Rotary_direction = 0;// encoder0PinA = 10;
int Rotary_count = 0;//encoder0PinB = 11;
int Rotary_switch = 0;
byte CounterTemp = 0;
int Rotary_directionLast = HIGH; //encoder0PinALast = LOW;
int Rotary_switchLast = HIGH; //encoderSwitchLast = LOW;
int Rotary_directionCurrent = HIGH; //n = LOW;


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Wire.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(Seconds_LED, OUTPUT);
  pinMode(Minutes_LED, OUTPUT);
  pinMode(Reset_Pin, OUTPUT); //reset for the multiplexer
  // initialize the rotary endoder inputs
  pinMode(Rotary_switch_pin, INPUT);
  pinMode(Rotary_count_pin, INPUT);
  pinMode(Rotary_direction_pin, INPUT);

  delay(200);
  digitalWrite(Reset_Pin,LOW);
  delay(200);
  digitalWrite(Reset_Pin,HIGH);
  delay(200);

  Serial.begin(9600);
  Serial.println("OCXO Clock");

  delay(200);

  tcaselect(0);
  if (! ledmatrix.begin()) {
    Serial.println("IS31 not found");
    while (1);
  }
  Serial.println("IS31 found!");

  tcaselect(1);
  if (! ledmatrix.begin()) {
    Serial.println("IS31 not found");
    while (1);
  }
  Serial.println("IS31 found!");

  tcaselect(2);
  if (! ledmatrix.begin()) {
    Serial.println("IS31 not found");
    while (1);
  }
  Serial.println("IS31 found!");

  tcaselect(3);
  if (! ledmatrix.begin()) {
    Serial.println("IS31 not found");
    while (1);
  }
  Serial.println("IS31 found!");

  tcaselect(4);
  if (! ledmatrix.begin()) {
    Serial.println("IS31 not found");
    while (1);
  }
  Serial.println("IS31 found!");

  tcaselect(5);
  if (! ledmatrix.begin()) {
    Serial.println("IS31 not found");
    while (1);
  }
  Serial.println("IS31 found!");
  cli();//stop interrupts
  //set timer1 interrupt at 5Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 62499;// = (10*10^6) / 16 - 1 (must be <65536) Max we can do, so running at 10 Hz 
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1 prescaler
  //TCCR1B |= (1 << CS12) | (1 << CS10);  
  // External clock source on T1 pin. Clock on rising edge. This doesn't use the prescalar - unfortunately!
  TCCR1B |=  (1<<CS10) ;// External clock source on T1 pin. Clock on rising edge. PD5, no prescalar
  TCCR1B |=  (1<<CS11) ;
  TCCR1B |=  (1<<CS12) ;
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  
  sei();//allow interrupts
  
}

// the loop function runs over and over again forever
void loop() {
  /*Debouncedelay = 0;
  while (Debouncedelay < 65000) {
    Debouncedelay++;
  } */
  //Check the switch
  Switch_current = digitalRead(Rotary_switch_pin);
  if ((Switch_last == HIGH) && (Switch_current == LOW)) {
    Debouncedelay = 0;
    while (Debouncedelay < 65000) {
      Debouncedelay++;
    } 
    Switch_position++;
    if (Switch_position == 7) {
      Switch_position = 0;
    }
    //Serial.println (encoder0Pos);
    }  
  Switch_last = Switch_current;

  //selects the digit for setting the time
  if (Switch_position == 0){
    HoursTensSelect = 0;
    HoursUnitsSelect = 0;
    MinutesTensSelect = 0;
    MinutesUnitsSelect = 0;
    SecondsTensSelect = 0;
    SecondsUnitsSelect = 0;
  }

  if (Switch_position == 1){
    HoursTensSelect = 10;
    HoursUnitsSelect = 0;
    MinutesTensSelect = 0;
    MinutesUnitsSelect = 0;
    SecondsTensSelect = 0;
    SecondsUnitsSelect = 0;

    //Read the rotary counter
    CounterTemp = HoursTens;
    RotaryEncoder();
    HoursTens = CounterTemp;
    if (HoursTens > 2){
      HoursTens = 0;
    }
  }

  if (Switch_position == 2){
    HoursTensSelect = 0;
    HoursUnitsSelect = 10;
    MinutesTensSelect = 0;
    MinutesUnitsSelect = 0;
    SecondsTensSelect = 0;
    SecondsUnitsSelect = 0;

    //Read the rotary counter
    CounterTemp = HoursUnits;
    RotaryEncoder();
    HoursUnits = CounterTemp;
    if (HoursUnits > 9){
      HoursUnits = 0;
    }
    if ((HoursUnits > 4) && (HoursTens = 2)){
      HoursUnits = 0;
    }

  }

  if (Switch_position == 3){
    HoursTensSelect = 0;
    HoursUnitsSelect = 0;
    MinutesTensSelect = 10;
    MinutesUnitsSelect = 0;
    SecondsTensSelect = 0;
    SecondsUnitsSelect = 0;

     //Read the rotary counter
    CounterTemp = MinutesTens;
    RotaryEncoder();
    MinutesTens = CounterTemp;
    if (MinutesTens > 6){
      MinutesTens = 0;
    }   

  }

  if (Switch_position == 4){
    HoursTensSelect = 0;
    HoursUnitsSelect = 0;
    MinutesTensSelect = 0;
    MinutesUnitsSelect = 10;
    SecondsTensSelect = 0;
    SecondsUnitsSelect = 0;

    //Read the rotary counter
    CounterTemp = MinutesUnits;
    RotaryEncoder();
    MinutesUnits = CounterTemp;
    if (MinutesUnits > 9){
      MinutesUnits = 0;
    }   

  }

  if (Switch_position == 5){
    HoursTensSelect = 0;
    HoursUnitsSelect = 0;
    MinutesTensSelect = 0;
    MinutesUnitsSelect = 0;
    SecondsTensSelect = 10;
    SecondsUnitsSelect = 0;

     //Read the rotary counter
    CounterTemp = SecondsTens;
    RotaryEncoder();
    SecondsTens = CounterTemp;
    if (SecondsTens > 6){
      SecondsTens = 0;
    }   

  }

  if (Switch_position == 6){
    HoursTensSelect = 0;
    HoursUnitsSelect = 0;
    MinutesTensSelect = 0;
    MinutesUnitsSelect = 0;
    SecondsTensSelect = 0;
    SecondsUnitsSelect = 10;
 
    //Read the rotary counter
    CounterTemp = SecondsUnits;
    RotaryEncoder();
    SecondsUnits = CounterTemp;
    if (SecondsUnits > 9){
      SecondsUnits = 0;
    }   

  }

  if (toggle2){
    //Drive the display
   tcaselect(0);
    for (x = 0; x < 15; x++) // across the x axis, x is declared as a variable here
      for (y = 0; y < 7; y++) // across the y axis, y is declared as a variable here
        ledmatrix.drawPixel(x, y, (brightness + HoursTensSelect) * (digits[((x + (15 * (HoursTens))) * 7) + (6 - y)]));
    tcaselect(1);
    for (x = 0; x < 15; x++) // across the x axis, x is declared as a variable here
      for (y = 0; y < 7; y++) // across the y axis, y is declared as a variable here
        ledmatrix.drawPixel(x, y, (brightness + HoursUnitsSelect) * (digits[((x + (15 * (HoursUnits))) * 7) + (6 - y)]));
    //display the SecondsTens and SecondsUnits
    tcaselect(2);
    for (x = 0; x < 15; x++) // across the x axis, x is declared as a variable here
      for (y = 0; y < 7; y++) // across the y axis, y is declared as a variable here
        ledmatrix.drawPixel(x, y, (brightness + MinutesTensSelect) * (digits[((x + (15 * (MinutesTens))) * 7) + (6 - y)]));
    tcaselect(3);
    for (x = 0; x < 15; x++) // across the x axis, x is declared as a variable here
      for (y = 0; y < 7; y++) // across the y axis, y is declared as a variable here
        ledmatrix.drawPixel(x, y, (brightness + MinutesUnitsSelect) * (digits[((x + (15 * (MinutesUnits))) * 7) + (6 - y)]));
    tcaselect(4);
    for (x = 0; x < 15; x++) // across the x axis, x is declared as a variable here
      for (y = 0; y < 7; y++) // across the y axis, y is declared as a variable here
        ledmatrix.drawPixel(x, y, (brightness + SecondsTensSelect) * (digits[((x + (15 * (SecondsTens))) * 7) + (6 - y)]));
    tcaselect(5);
    for (x = 0; x < 15; x++) // across the x axis, x is declared as a variable here
      for (y = 0; y < 7; y++) // across the y axis, y is declared as a variable here
        ledmatrix.drawPixel(x, y, (brightness + SecondsUnitsSelect) * (digits[((x + (15 * (SecondsUnits))) * 7) + (6 - y)]));
  }      
  //delay(20);
}

void RotaryEncoder(){
  Debouncedelay = 0;
  while (Debouncedelay < 65000) {
    Debouncedelay++;
  } 
  Rotary_directionCurrent = digitalRead(Rotary_direction_pin);
  if ((Rotary_directionLast == HIGH) && (Rotary_directionCurrent == LOW)) {
    Debouncedelay = 0;
    while (Debouncedelay < 65000) {
      Debouncedelay++;
    } 
    if (digitalRead(Rotary_count_pin) == LOW) {
      CounterTemp--;
    } else {
      CounterTemp++;
    }
    Serial.println (CounterTemp);
  }
  Rotary_directionLast = Rotary_directionCurrent;
  Temp = CounterTemp;
}

ISR(TIMER1_COMPA_vect){
  
  if (Count == 9){ 
      Count = 0;
      SecondsUnits++;
      if (SecondsUnits >= 10){
      SecondsUnits = 0;
      SecondsTens++;
      }
      if (SecondsTens >= 6){
          SecondsTens = 0;
          MinutesUnits++;
      }
      if (MinutesUnits >= 10){
          MinutesUnits = 0;
          MinutesTens++;
      }
      if (MinutesTens >= 6){
          MinutesTens = 0;
          HoursUnits++;
      }
      if (HoursUnits >= 10){
          HoursUnits = 0;
          HoursTens++;
      }
      if (HoursUnits == 4 && HoursTens == 2){
          HoursUnits = 0;
          HoursTens = 0;
      }
      //Serial.println(SecondsUnits);
      /*Serial.print(HoursTens); 
      Serial.print(HoursUnits); 
      Serial.print(":"); 
      Serial.print(MinutesTens);
      Serial.print(MinutesUnits);
      Serial.print(":");
      Serial.print(SecondsTens);
      Serial.println(SecondsUnits);*/
      if (toggle2){
        digitalWrite(LED_BUILTIN,HIGH);
        digitalWrite(Seconds_LED,HIGH);
        digitalWrite(Minutes_LED,HIGH);
        toggle2 = 0;
      }
      else{
        digitalWrite(LED_BUILTIN,LOW);
        digitalWrite(Seconds_LED,LOW);
        digitalWrite(Minutes_LED,LOW);
        toggle2 = 1;
      }
    }
  else{
     Count++;
  }
}
void tcaselect(uint8_t i) {
  //digitalWrite(LED_BUILTIN,HIGH);
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission(); 
}
