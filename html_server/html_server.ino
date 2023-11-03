#include <WiFiClient.h>
#include <WiFiServer.h>
#include <ESP8266WiFi.h>

//----------------------------------------------------------------------variaveis
IPAddress NETWORK_IP(192, 168, 2, 100);
IPAddress GATE(192, 168, 2, 100);
IPAddress SUB(255, 255, 0, 0);
const char V3[] = "shadow";
const char V4[] = "Titanico";
int R = 0;
int counter;
int T;
String TN = " HTTP/1.1";
String FN;
String FS;
String nmnt = "Valentim";
String ntps = "23011998";
String Command = "1";
int msgvl = 0;
int msgtt = 0;
String msg[16];
IPAddress IPNT;
IPAddress IPAP;

//complementos
WiFiServer  server(80);
//----------------------------------------------------------------------setup
void setup() {
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  Serial.begin(115200);
  WiFi.softAP("Shadow", "Titanico");

  server.begin();

  IPAP = WiFi.softAPIP();
  Serial.println();
  Serial.print("connect in: ");
  Serial.println( IPAP );
  Serial.print("connect in:");
  Serial.println( IPNT );
}
//----------------------------------------------------------------------executado
void loop() {
  //redescan
  int n = WiFi.scanNetworks();

  if (Command == "1") {
    for (counter = 0; counter < 9; ++counter) {
      Serial.println("");
    }

    WiFi.disconnect();
    WiFi.begin(nmnt, ntps);
    WiFi.config(NETWORK_IP, GATE, SUB);

    while ( WiFi.status() != WL_CONNECTED ) {
      Serial.print("|-|");
      digitalWrite(D4, HIGH);
      delay(1000);
      Serial.print("|-|");
      digitalWrite(D4, LOW);
      delay(1000);
    }
    digitalWrite(D4, HIGH);

    IPNT = WiFi.localIP();
    Serial.println();
    Serial.print("connect in:");
    Serial.println( IPNT );
    Command = "0";
  }

  WiFiClient client = server.available();
  String request = client.readStringUntil('\r');
  if (Serial.available() != 0) {
    R = Serial.read();
    Serial.print("local command:");
    Serial.println(int(R));
  }/*
  delay(3000);
  Serial1.print("Serial1");
  Serial.print("Serial");*/
  if ( request.length() != 0 ) {
    Serial.print(request);
    Serial.println("|");
    //interno semi-bruto
    FN = request.substring(6);
    T = FN.length() - TN.length();
    FS = FN.substring(0, T);
    Serial.println(FS); Serial.println("|");
    //processamento de respostar
  }
  if (request == "GET /?cmd=menu HTTP/1.1") {
    R = 0;
    Serial.println("OpenMenu"); Serial.println("|");
  }
  if (request == "GET /?cmd=acessar HTTP/1.1") {
    R = 1;
    Serial.println("OpenLogin"); Serial.println("|");
  }
  if (request == "GET /?cmd=dados HTTP/1.1") {
    R = 3;
    Serial.println("OpenDados"); Serial.println("|");
  }
  if (request.startsWith("GET /?pass=")) {
    R = 2;
    nmnt = FS.substring(5);
    Serial.println(nmnt); Serial.println("|");
    Serial.println("OpenPass"); Serial.println("|");
  }
  if (request.startsWith("GET /?ntps=")) {
    ntps = FS.substring(5);
    Serial.println(ntps); Serial.println("|");
    Serial.println("OpenNtps"); Serial.println("|");
    Command = "1" ;
  }
    if (request.startsWith("GET /?msg=")) {
      if(msgtt == 16){
        msgtt = 0;
      }
    ++msgtt;
    msg[msgtt] = FS.substring(4);
    Serial.println("menssge_recive_is:"); Serial.println("|"); 
    Serial.println(msg[msgtt]); Serial.println("|"); 
    
  }
  //carregamento do FrontEnd
  //1page
  switch (R) {
    case 0:
      client.println("<!DOCTYPE html>"
                     "<html style=\"background-color:gray;\">"
                     "<title>Silo sistem</title>"
                     "<head>"
                    );

      if ( WiFi.status() == WL_CONNECTED ) {
        client.println("<h3 style=\"color:green;font-size:50%;\">Connected in</h3>");
        client.println("<h3 style=\"color:green;font-size:50%;\">"); client.println(nmnt); client.println("</h3>");
      } else {
        client.println("<h3 style=\"color:red;font-size:50%;\">Not Connected</h3>");
      }

      client.println(
                     "<center>"
                     "<img width=\"200\" src=\"https://lh3.googleusercontent.com/pw/AMWts8APXubyYKrktmiR2v3lWup4Y5WyvyIiB0-8MTvO3EMP-5c3TpDjl01PwLNTzQVMrWw2z_GiC7--P5BfW7WnbQHHecp0PAUl5Ta3xvFQuFmBaAi5ruRCGyd36CIuaMaJievomx9mYS2whghFb046Q929=w568-h757-no?authuser=0\">"
        "</center>"
        "</head>"
        "<center>"
        "<h1 style=\"color:darkgreen;font-size:300%;\">Hello</h1>"
        "</center>"
        "<body>"
        "<center>"
        "<form><h3 style=\"color:darkgreen;font-size:150%;\">configuracao de rede</h3>"
        "<input type=\"submit\" style=\"color:orange;font-size:200%;background-color:gray;\" value=\"acessar\" name=\"cmd\"></form>"
        "<form><h3 style=\"color:darkgreen;font-size:150%;\">info</h3>"
        "<input type=\"submit\" style=\"color:orange;font-size:200%;background-color:gray;\" value=\"dados\" name=\"cmd\">"
        "</form></center>"
        "</body>"
        "</html>"
      );
      break;
    case 1://2page/Network
      client.println("<!DOCTYPE html>"
                     "<html style=\"background-color:gray;\">"
                     "<head>"
                     "<form>"
                     "<input type=\"Submit\" name=\"cmd\" value=\"menu\"style=\"background-color:gray;color:orange;\">"
                     "</form>"
                     "<meta http-equiv=\"CONTENT-TYPE\" content=\"text/html; charset=UTF-8\">"
                     "<title>Silo sistem/Network</title>"
                     "</head>"
                     "<body>"
                     "<center>"
                     "<h1 style=\"color:green;font-size:300%;\">Cadastro de Rede/Nome</h1>"
                     "<p style=\"color:darkgreen;font-size:150%;\"> Selecione a Rede</p>"
                    );

      for (int i = 0; i < n; ++i) {
        client.print("<form><input type=\"Submit\" name=\"");
        client.print("pass");
        client.print("\" value=\"");
        client.print(WiFi.SSID(i));
        client.print("\" style=\"color:orange;font-size:150%;background-color:gray;\"></form>");
        delay(1);
      }
      client.println(
      );
      if ( WiFi.status() == WL_CONNECTED ) {
        client.println("<h3 style=\"color:green;font-size:300%;\">Connected in</h3>");
        client.println("<h3 style=\"color:green;font-size:300%;\">"); client.println(nmnt); client.println("</h3>");
      } else {
        client.println("<h3 style=\"color:red;font-size:300%;\">Not Connected</h3>");
      }

      client.println(
        "</center>"
        "</body>"
        "</html>"
      );
      break;
    case 2://page4/Pass
      client.println("<!DOCTYPE html>"
                     "<html style=\"background-color:gray;\">"
                     "<head>"
                     "<form>"
                     "<input type=\"Submit\" name=\"cmd\" value=\"menu\"style=\"background-color:gray;color:orange;\">"
                     "</form>"
                     "<meta http-equiv=\"CONTENT-TYPE\" content=\"text/html; charset=UTF-8\">"
                     "<title>Silo sistem/Pass</title>"
                     "</head>"
                     "<body>"
                     "<center>"
                     "<h1 style=\"color:green;font-size:300%;\">Cadastro de Rede/Senha</h1>"
                     "<form>"
                     "<p><label style=\"color:green;font-size:300%;\">password</label>"
                     "<input type=\"password\" name=\"ntps\" >"
                     "</p>"
                     "<input type=\"Submit\" placeholder=\"Submit\" style=\"background-color:gray;color:orange;font-size:200%\">"
                     "</form>");
      if ( WiFi.status() == WL_CONNECTED ) {
        client.println("<h3 style=\"color:green;font-size:300%;\">Connected in</h3>");
        client.println("<h3 style=\"color:green;font-size:300%;\">"); client.println(nmnt); client.println("</h3>");
      } else {
        client.println("<h3 style=\"color:red;font-size:300%;\">Not Connected</h3>");
      }

      client.println(
        "</center>"
        "</body>"
        "</html>");
      break;
    case 3://page3/Dados
      client.println("<!DOCTYPE html>"
                     "<html style=\"background-color:gray;\">"
                     "<head>"
                     "<form>"
                     "<input type=\"Submit\" name=\"cmd\" value=\"menu\"style=\"background-color:gray;color:orange;\">"
                     "</form>"
                     "<meta http-equiv=\"CONTENT-TYPE\" content=\"text/html; charset=UTF-8\">"
                     "<title>Silo sistem/Dados</title>"
                     "</head>"
                     "<body>"
                     "<center>"
                     "<h1 style=\"font-size:500%;color:red;\">In Workprogress</h1>"
                     "<form>"
                     "<input type=\"text\" name=\"msg\" placeholder=\"your menssage\"style=\"background-color:white;color:red;font-size:150%;\">"
                     "<form>");
for(msgvl = msgtt; msgvl == 0;){
client.print("<p>");client.print(msg[msgvl]);client.println("</p>");
   --msgvl;
}
client.println(
                     "</center>"
                     "</body>"
                     "</html>");
      break;
  }
}
