#include <IOXhop_FirebaseESP32.h> //biblioteca de integração do firebase
#include <IOXhop_FirebaseStream.h>

#include <WiFi.h> //biblioteca do wifi
#include "time.h" //biblioteca de informações de horas

#define tamanho_rotina 60 //quantidade de horarios disponiveis para programar

#define ssid  "Ranadaia"; //ssid do wifi
#define password "D@vi2410"; //senha do wifi
#define firebase_host "https://dispensador-remedios-default-rtdb.firebaseio.com/" //host do firebase 
#define firebase_auth "token_or_secret" //chave de autenticação do firebase

//estrutura de dados para armazenar dados da rotina
struct DadosRotina {  
  int hora;
  int minuto;
  int slot;
  char* nome;
  };

const char* ntpServer = "0.br.pool.ntp.org"; //endereço do servidor de tempo
const long  gmtOffset_sec = -3 * 3600; //config do fuso horario br
const int   daylightOffset_sec = 0; //horário de verão

/*
DadosRotina horario_rotina[tamanho_rotina] = {
  
  };


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
bool CHECA_HORARIO(int rotina[tamanho_rotina][2])
{
  for(int i = 0; i < 10; i++)
   {
      if(get_LocalTimeHora() == rotina[i][0] && get_LocalTimeMinuto() == rotina[i][1])
      {
        Serial.println("DEU O HORÁRIO");
        return true;
      }
   }
   return false;
}
/***************************************************************************************/
void CONECTA_WIFI() //Função para conectar no wifi
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
void MOSTRA_HORARIO()
{
  Serial.print((String)get_LocalTimeHora() + " : " + (String)get_LocalTimeMinuto() + '\n');
}
/***************************************************************************************/


void setup() //Função de configuração
{
  Serial.begin(115200);
  
  CONECTA_WIFI();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); //Conecta no servidor e obtem o horario local
  MOSTRA_LocalTime();

}

void loop() //Função principal de execução
{
  MOSTRA_HORARIO();
  Serial.println('.');
  delay(1000);

  //Checa firebase
  
  //Verifica rotina
  if (CHECA_HORARIO(horario_rotina))
  {
    //aciona os motores dos compartimentos
  }
  
}
