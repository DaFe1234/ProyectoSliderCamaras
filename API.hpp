void setGPIO(String id, bool state)
{
   /*Serial.print("Set GPIO ");
   Serial.print(id);
   Serial.print(": ");
   Serial.println(state);*/
   if(id == "D6"){
    pinMode(D6, OUTPUT);
    digitalWrite(D6, state);
    Serial.println("a");
   }
   if(id == "D7"){
    pinMode(D7, OUTPUT);
    digitalWrite(D7, state);
    Serial.println("e");
   }
   if(id == "D8"){
    pinMode(D8, OUTPUT);
    digitalWrite(D8, state);
   }
}

void setPWM(String id, int pwm)
{
   Serial.print("Set PWM ");
   Serial.print(id);
   Serial.print(": ");
   Serial.println(pwm);
   if(id == "1"){
    float valor = 0.0;
    valor = pwm*2.41/360;
    String comando = "G21G91Y"+ String(valor, 2) +"F1";
    Serial.println(comando);
   }
}

void doAction(String actionId)
{
   Serial.print("Doing action: ");
   Serial.println(actionId);
}
