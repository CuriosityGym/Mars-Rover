int ch1,ch2,ch3,ch4,ch5,ch6;
  

void setup() {
  pinMode(A0, INPUT);//A0=14 and so on.
  pinMode(A1, INPUT); 
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  Serial.begin(9600); 
  Serial.print("\t");
  Serial.println("Ch1:\tCh2t:\tCh3:\tCh4:\tCh5:\tCh6:\t");
}

void loop() {
  ch6 = pulseIn(A5, HIGH);
  ch5 = pulseIn(A4, HIGH);
  ch4 = pulseIn(A3, HIGH); 
  ch3 = pulseIn(A2, HIGH);
  ch2 = pulseIn(A1, HIGH);
  ch1 = pulseIn(A0, HIGH); 
  Serial.print("\t");
  Serial.print(ch1);
  Serial.print("\t");       
  Serial.print(ch2);
  Serial.print("\t");       
  Serial.print(ch3);
  Serial.print("\t");       
  Serial.print(ch4);
  Serial.print("\t");       
  Serial.print(ch5);
  Serial.print("\t");       
  Serial.println(ch6); 
  delay(2000);
}

