
void setup(){
  for (int i=2; i<=17; i++)
  {
    pinMode(i, INPUT);
  }
}

void loop(){
  for (int i=10; i<18; i++)
  {
    int power = digitalRead(i);
    if (power = LOW){
      Serial.println(i);
    //do something 
    }
  }
  
}