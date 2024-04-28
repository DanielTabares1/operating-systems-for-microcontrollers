const int g = 13, r = 12;


void setup() {
  Serial.begin(9600);
  delay(1000);
  pinMode(g, OUTPUT);
  pinMode(r, OUTPUT);

  xTaskCreatePinnedToCore(Tarea1,"Tarea1",10000,NULL,1,NULL,0);
  xTaskCreatePinnedToCore(Tarea2,"Tarea2",10000,NULL,1,NULL,1);

}

void loop() {
  delay(1000);
}

void Tarea1( void * parameter )
{
    while(1){
        Serial.print("Tarea 1 - ROJO - corriendo en core ");
        Serial.println(xPortGetCoreID());
        digitalWrite(r, HIGH);
        delay(1000);
        digitalWrite(r, LOW);
        delay(1000);
    }
}

void Tarea2( void * parameter)
{
    while(1){
        Serial.print("Tarea2 - VERDE - corriendo en core ");
        Serial.println(xPortGetCoreID());
        digitalWrite(g, HIGH);
        delay(700);
        digitalWrite(g, LOW);
        delay(700);
    }
}
