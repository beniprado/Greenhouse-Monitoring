#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define ldr A0
#define dht 6
#define pinled 7
#define pinbutton 8
//#define pinSensorSL A2 em breve!

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht11(dht, DHT11);


int statebutton = 0; // Variável responsável por armazenar o estado do botão (ligado/desligado)
int valorSl;
//int monito2;
int valor1;
int monito1;
bool alert = false; // Variável para rastrear se o aviso foi enviado
int displayState = 0; // Estado atual do display (0, 1 ou 2)
int buttonPressed = false;// Rastreamento de pressionamento do botão

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);  // Especifica o número de colunas e linhas
  dht11.begin();
  pinMode(pinbutton, INPUT);
  pinMode(ldr, INPUT);
  pinMode(pinled, OUTPUT);
  //pinMode(pinSensorSL, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("       B+      ");
  delay(1500);
  lcd.clear();
}

void loop() {
  float humi = dht11.readHumidity();
  float temC = dht11.readTemperature();
  statebutton = digitalRead(pinbutton);
  valor1 = analogRead(ldr);
//  valorSl = analogRead(pinSensorSL);
 float monito2 = map(valorSl, 0, 1023, 0, 100);
  monito1 = map(valor1, 0, 1023, 0, 1000);

  if (statebutton == HIGH && !buttonPressed) {
    buttonPressed = true;
    displayState = (displayState + 1) % 4; //Alterna entre 0,1 e 2
  } else if (statebutton == LOW) {
    buttonPressed = false;
  }

  Serial.print(valorSl);

  switch (displayState) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print("  Temperatura:  ");
      lcd.setCursor(0, 1);
      lcd.print(temC);
      lcd.print(" C      1/3");
      break;
    case 1:
      lcd.setCursor(0, 0);
      lcd.print("    Humidade:  ");
      lcd.setCursor(0, 1);
      lcd.print(humi);
      lcd.print(" %      2/3");
      break;
    case 2:
      lcd.setCursor(0, 0);
      lcd.print("    Lumens:   ");
      lcd.setCursor(0, 1);
      lcd.print(monito1);
      lcd.print(" lumens   3/3 ");
      break;
    /*case 3: 
      lcd.setCursor(0,0);
      lcd.print(" Humidade Solo: ");
      lcd.setCursor(0, 1);
      lcd.print(monito2);
      lcd.print(" %        4/4 ");
      break;*/
  }


  if (monito1 < 335 && !alert) {
    lcd.setCursor(0, 1);
    lcd.print(" ---LED LIGADO---");
    digitalWrite(pinled, HIGH);
    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print("                "); // Limpa a linha
    alert = true;
  } else if (monito1 >= 345 && alert) {
    digitalWrite(pinled, LOW);
    alert = false;
  }
}
