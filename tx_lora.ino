#define ledPin 2
#define pSwitch 13
int toggleState_1 = 0; //Define integer to remember the toggle state

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(pSwitch, INPUT);  
}

void loop() {
  // put your main code here, to run repeatedly:

  if (digitalRead(pSwitch) == HIGH){
    if (toggleState_1 == 0) {
      Serial.println("AT+SEND=1,6,LEDON");
      digitalWrite(ledPin, HIGH);
      toggleState_1 = 1;
      delay(500);
    }
    else {
      Serial.println("AT+SEND=1,6,LEDOFF");
      digitalWrite(ledPin, LOW);
      toggleState_1 = 0;
      delay(500);
    }
  }
  
}
