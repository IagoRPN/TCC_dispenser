#include <WiFi.h>
#include "time.h"

const char* ssid       = "";
const char* password   = "";

const char* ntpServer = "0.br.pool.ntp.org";
const long  gmtOffset_sec = -3 * 3600;
const int   daylightOffset_sec = 0;

int horario_rotina[10][2] = {
  {1,35}

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
void CHECA_HORARIO(int hora_aux, int min_aux, int rotina[10][2])
{
   for(int i = 0; i < 10; i++)
   {
      if(hora_aux == rotina[i][0] && min_aux <= rotina[i][1])
      {
        Serial.println("DEU O HORÁRIO");
      }
   }
}

/***************************************************************************************/
/*
void GUARDA_HORARIO(int hora_aux, int min_aux)
{
  
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
void DESCONECTA_WIFI() //Função para desconectar do wifi
{
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}
/***************************************************************************************/
void MOSTRA_UnixTime() //Função para mostrar no monitor serial o tempo no sistema unix
{
  time_t seconds;
  Serial.println(time(&seconds));
}
/***************************************************************************************/
long get_UnixTime() //Função para obter o tempo no sistema Unix
{
  time_t seconds;
  return time(&seconds);
}
/***************************************************************************************/

/***************************************************************************************/
void setup() //Função de configuração
{
  Serial.begin(115200);
  
  CONECTA_WIFI();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); //Conecta no servidor e obtem o horario local
  MOSTRA_LocalTime();
  DESCONECTA_WIFI();

}

void loop() //Função principal de execução
{
  //MOSTRA_UnixTime();
  int h = get_LocalTimeHora();
  int m = get_LocalTimeMinuto();
  CHECA_HORARIO(h,m,horario_rotina);
  //Serial.print((String)h + ':' + (String)m + '\n');
  //printLocalTime();
  Serial.println('.');
  delay(1000);
  
}
