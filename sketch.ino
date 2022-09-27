#include <IOXhop_FirebaseESP32.h>
#include <IOXhop_FirebaseStream.h>

#include <WiFi.h>

#define  wifi_ssid  "Ranadaia" 
#define  wifi_auth "D@vi2410"

#define firebase_host "https://projeto-de-teste-f47a5-default-rtdb.firebaseio.com/"
#define firebase_auth "zRhK7zEL1QNtupJ8YwDdb9amQEkZbKAqzFZiaWqJ"

const int tam_list = 4;

typedef struct  {
    int idade;
    String nome;
} data_list;

 data_list DL[tam_list];

void AtualizaLista() {
  for (int i = 0; i <= tam_list - 1; i++){
    DL[i].idade = Firebase.getInt("/list/"+(String)i+"/idade");
    DL[i].nome =  Firebase.getString("/list/"+(String)i+"/nome");
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  Serial.begin(115200);
  
  WiFi.begin(wifi_ssid, wifi_auth);
  while (WiFi.status() != WL_CONNECTED){
    Serial.println(".");
  }

  Serial.println("Connected to: " + WiFi.SSID());
  digitalWrite(2, HIGH);

  Firebase.begin(firebase_host, firebase_auth);
} 


void loop() {
  // put your main code here, to run repeatedly:

  AtualizaLista();
  
  Serial.println(
    (String)DL[0].idade + '\n' +
    DL[0].nome + '\n' +
    (String)DL[1].idade + '\n' +
    DL[1].nome + '\n' +
    (String)DL[2].idade + '\n' +
    DL[2].nome + '\n' +
    (String)DL[3].idade + '\n' +
    DL[3].nome + '\n'
  );

  delay(50);

}
