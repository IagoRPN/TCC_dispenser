#include <IOXhop_FirebaseESP32.h> //biblioteca de integração do firebase
#include <IOXhop_FirebaseStream.h>

#include <WiFi.h> //biblioteca do wifi
#include "time.h" //biblioteca de informações de horas


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
#define PINO_EN5 4
#define PINO_EN6 2
#define PINO_SENSOR 36
#define PINO_LCD_TX 23
#define PINO_LCD_RX 22
#define PINO_BOTAO 39
#define PINO_BUZZER 15

const char* ntpServer = "0.br.pool.ntp.org"; //endereço do servidor de tempo
const long  gmtOffset_sec = -3 * 3600; //config do fuso horario br
const int   daylightOffset_sec = 0; //horário de verão

const int tam_list = 2;

//estrutura de dados para armazenar dados da rotina
typedef struct {
  int hora;
  int minuto;
  int slot;
  String nome;
} DadosRemedio;

DadosRemedio rotina[tam_list];

void AtualizaLista() {
  for (int i = 0; i <= tam_list - 1; i++){
   rotina[i].hora = Firebase.getInt("/"+String(i)+"/hora");
   rotina[i].minuto = Firebase.getInt("/"+String(i)+"/hora");
  }
}

/***************************************************************************************/
void MOSTRA_LocalTime() //Função para imprimir o tempo local
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo))
  {
    Serial.println("Falha ao obter tempo");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}
/***************************************************************************************/
int get_LocalTimeHora()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo))
  {
    Serial.println("Falha ao obter tempo");
    return 0;
  }
  return timeinfo.tm_hour;
}

int get_LocalTimeMinuto()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo))
  {
    Serial.println("Falha ao obter tempo");
    return 0;
  }
  return timeinfo.tm_min;
}
/***************************************************************************************/
bool ChecaHorario(int rotina[tam_list])
{
  for(int i = 0; i < 10; i++)
   {
      if(get_LocalTimeHora() == rotina[i].hora && get_LocalTimeMinuto() == rotina[i].minuto)
      {
        Serial.println("DEU O HORÁRIO");
        return true;
      }
   }
   return false;
}
/***************************************************************************************/
void ConectaWiFi() //Função para conectar no wifi
{
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
}
/***************************************************************************************/
void MostraHorario()
{
  Serial.print((String)get_LocalTimeHora() + " : " + (String)get_LocalTimeMinuto() + '\n');
}
/***************************************************************************************/


void setup() //Função de configuração
{
  Serial.begin(115200);
  
  ConectaWiFi();
  Firebase.begin(firebase_host, firebase_auth);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); //Conecta no servidor e obtem o horario local
  MOSTRA_LocalTime();

}

void loop() //Função principal de execução
{
  AtualizaLista();
  MostraHorario();
  Serial.println('.');
  delay(1000);

  //Checa firebase
  
}
  
}
