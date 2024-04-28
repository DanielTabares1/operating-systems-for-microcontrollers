const int r = 12, g = 13;


void setup() {
  Serial.begin(9600);
  delay(1000);
  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);

  xTaskCreate(Tarea1,"Tarea1",10000,NULL,1,NULL);
  xTaskCreate(Tarea2,"Tarea2",10000,NULL,1,NULL);

}

void loop() {
  delay(1000);
}

void Tarea1( void * parameter )
{
    while(1){
        Serial.println("Tarea 1 - ROJO");
        digitalWrite(r, HIGH);
        delay(1000);
        digitalWrite(r, LOW);
        delay(1000);
    }
}

void Tarea2( void * parameter)
{
    while(1){
        Serial.println("Tarea2 - VERDE");
        digitalWrite(g, HIGH);
        delay(700);
        digitalWrite(g, LOW);
        delay(700);
    }
}
