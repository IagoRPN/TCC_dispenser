//Bibliotecas utilizadas
#include <Stepper.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <Wire.h>
#include <IOXhop_FirebaseESP32.h> 
#include <IOXhop_FirebaseStream.h>
#include "time.h" 

//Definição dos nomes
#define ssid  "Ranadaia" //ssid do wifi
#define password "D@vi2410" //senha do wifi
#define firebase_host "https://dispensador-remedios-default-rtdb.firebaseio.com/" //host do firebase 
#define firebase_auth "pLRZuYhSqpWbvoV6fPs9rgpc3PqRi79qAiOhcCC3" //chave de autenticação do firebase

//Definição dos pinos
#define PINO_MOTOR1 13
#define PINO_MOTOR2 12
#define PINO_MOTOR3 14
#define PINO_MOTOR4 27
#define PINO_EN1 26
#define PINO_EN2 25
#define PINO_EN3 33
#define PINO_EN4 32
#define PINO_SENSOR 36
#define PINO_LCD_TX 23
#define PINO_LCD_RX 22
#define PINO_BOTAO 39
#define PINO_BUZZER 15

//Definição das variaveis do motor
#define SPEED 30
#define PASSOS 4096

LiquidCrystal_I2C lcd(0x27,20,4);
Stepper motor(64, PINO_MOTOR1, PINO_MOTOR2 ,PINO_MOTOR3 ,PINO_MOTOR4);

const char* ntpServer = "0.br.pool.ntp.org"; //endereço do servidor de tempo
const long  gmtOffset_sec = -3 * 3600; //config do fuso horario br
const byte   daylightOffset_sec = 0; //horário de verão

const byte tam_lista= 2; //Variavel com o tamanho da lista de remedios do paciente

//estrutura de dados para armazenar dados da rotina
typedef struct {
  int hora;
  int minuto;
  int slot;
  String nome;
  int quantidade;
} DadosRemedio;

DadosRemedio rotina[tam_lista];

void InicializaLCD(){
  lcd.init();
  lcd.backlight();
  lcd.setCursor(5,0);
  lcd.print("DISPENSER");
  lcd.setCursor(5,1);
  lcd.print("AUTOMÁTICO");
  lcd.setCursor(3,2);
  lcd.print("ENG. ELÉTRICA");
  lcd.setCursor(8,3);
  lcd.print("UNIP");
}

void MostraMenu(){
  lcd.clear();
  //Mostra Horário
  lcd.setCursor(15,0);
  lcd.print(String(get_LocalTimeHora()) + ":" + String(get_LocalTimeMinuto()));
  //
}

char* comando = "n";

char* Leitura_Botao(int porta){
  int l1, l2, l3, l4, leitura = 0;
  l1 = analogRead(porta);
  l2 = analogRead(porta);
  l3 = analogRead(porta);
  l4 = analogRead(porta);

  leitura = (l1+l2+l3+l4)/4;

  if (leitura == 0){
    comando = "SW1";
  }
  else if (leitura > 0 && leitura <= 100){
    comando = "SW2";
  }
  else if (leitura >= 200 && leitura <= 400){
    comando = "SW3";
  }
  else if (leitura >= 600 && leitura <= 800){
    comando = "SW4";
  }
  else if (leitura >= 1800 && leitura <= 2400){
    comando = "SW5";
  }
  else{
    comando = "n";
  }
  return comando;
}

void AtualizaLista() {
  for (int i = 0; i <= tam_lista - 1; i++){
   rotina[i].hora = Firebase.getInt("/"+String(i)+"/hora");
   rotina[i].minuto = Firebase.getInt("/"+String(i)+"/minuto");
   rotina[i].slot = Firebase.getInt("/"+String(i)+"/slot");
   rotina[i].nome = Firebase.getString("/"+String(i)+"/nome");
   rotina[i].quantidade = Firebase.getInt("/"+String(i)+"/quantidade");
  }
}

void AcionaBuzzer(){
  
}

void DesacionaBuzzer(){
  
}

void AcionaMotor(int porta_motor){
  motor.setSpeed(SPEED);
  if (porta_motor == 1){
    digitalWrite(PINO_EN1, HIGH);
    delay(100);
    motor.step(PASSOS);
    digitalWrite(PINO_EN1, LOW);
  }
    if (porta_motor == 2){
    digitalWrite(PINO_EN2, HIGH);
    delay(100);
    motor.step(PASSOS);
    digitalWrite(PINO_EN2, LOW);
  }
    if (porta_motor == 3){
    digitalWrite(PINO_EN3, HIGH);
    delay(100);
    motor.step(PASSOS);
    digitalWrite(PINO_EN3, LOW);
  }
    if (porta_motor == 4){
    digitalWrite(PINO_EN4, HIGH);
    delay(100);
    motor.step(PASSOS);
    digitalWrite(PINO_EN4, LOW);
  }
}

int get_LocalTimeHora(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo))
  {
    //Serial.println("Falha ao obter tempo");
    return 0;
  }
  return timeinfo.tm_hour;
}

int get_LocalTimeMinuto(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo))
  {
   // Serial.println("Falha ao obter tempo");
    return 0;
  }
  return timeinfo.tm_min;
}

void LiberaDoses(DadosRemedio rotina[tam_lista]) //Essa função recebe um array com todas as quantidades de doses 
{
  for(int i = 0; i < tam_lista ; i++)
   {
      if(get_LocalTimeHora() == rotina[i].hora && get_LocalTimeMinuto() == rotina[i].minuto)
      {
        //Avisa o usuario
        lcd.setCursor(0,1);
        lcd.print(rotina[i].nome);
        AcionaBuzzer();
        delay(1000);       
        if(Leitura_Botao(PINO_BOTAO) == "SW5"){ //Aguarda pressionar o botão
          for (byte i = 0; i < 2; i++){
            AcionaMotor(rotina[i].slot);
            if (PINO_SENSOR == LOW){
              lcd.setCursor(1,1);
              lcd.print("LIBERADO");
              return;
          }
         }
        }
      }
   }
}

void ConectaWiFi() //Função para conectar no wifi
{
  //Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      //Serial.print(".");
  }
  //Serial.println(" CONNECTED");
}

void setup() //Função de configuração
{
  Serial.begin(115200);
  //Desliga todos os pinos
  digitalWrite(PINO_EN1, LOW); 
  digitalWrite(PINO_EN2, LOW);
  digitalWrite(PINO_EN3, LOW);
  digitalWrite(PINO_EN4, LOW);
  InicializaLCD();
  delay(2000);
  ConectaWiFi();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("WIFI CONECTADO");
  Firebase.begin(firebase_host, firebase_auth);
  lcd.setCursor(0,1);
  lcd.print("FIREBASE CONECTADO");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); //Conecta no servidor e obtem o horario local
  //MOSTRA_LocalTime();
}

void loop() //Função principal de execução
{
  MostraMenu();
  AtualizaLista();
  LiberaDoses(rotina);
}
  
