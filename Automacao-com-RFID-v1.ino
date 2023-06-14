//Leitor RC522
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 53
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN);

//LEDs
#define vermelho 10
#define verde 9
#define azul 8

//Modulo de reles
#define IN1 24
int IN1status = HIGH;

//Delay
#define curto 500
#define normal 1000
#define longo 2000

void setup() {
  pinMode(verde, OUTPUT);
  pinMode(azul, OUTPUT);
  pinMode(vermelho, OUTPUT);
  pinMode(IN1, OUTPUT);
  digitalWrite(IN1, HIGH); //obs: o modelo de rele uitizado usa a logica de HIGH => desligado
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  boot();
}

void loop() {
  // verificando a presenca de cartoes
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  checaUID();
}

void checaUID() {
  Serial.print("UID: ");
  String uid = "";
  //conversao do valor hexadecimal puro para uma forma padronizada | obs.: o codigo utilizado para a conversao e identificacao do valor provem do seguinte local: https://randomnerdtutorials.com/security-access-using-mfrc522-rfid-reader-with-arduino/
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    uid.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    uid.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  uid.toUpperCase();
  //validacao do valor UID lido
  if (uid.substring(1) == "A0 13 C1 BC") {
    valido();
    delay(longo);
  } else {
    invalido();
  }
}

void valido() {
  Serial.println("Credencial valida");
  Serial.println();
  rele();
  digitalWrite(verde, HIGH);
  delay(curto);
  digitalWrite(verde, LOW);
}

void invalido() {
  Serial.println("Credencial invalida");
  Serial.println();
  digitalWrite(vermelho, HIGH);
  delay(curto);
  digitalWrite(vermelho, LOW);
}

void rele(){
  if(IN1status == HIGH){
    IN1status = LOW;
  } else IN1status = HIGH;
  digitalWrite(IN1, IN1status);
}

void boot() {
  Serial.println();
  Serial.println("O sistema foi inicializado.");
  digitalWrite(verde, HIGH);
  digitalWrite(azul, HIGH);
  digitalWrite(vermelho, HIGH);
  delay(normal);
  digitalWrite(verde, LOW);
  digitalWrite(azul, LOW);
  digitalWrite(vermelho, LOW);
}