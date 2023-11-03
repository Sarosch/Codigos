//Cria uma Função que Verifica a requisição e executa as devidas tarefas
void control(String data) {
  WiFiClient client = server.available();
  String manager;
  String ctrl;
  if (data.startsWith("net")) {
    ctrl = data.substring(3);
    net(ctrl);
    client.stop();
  }
  if (data.startsWith("Set base:")) {
    manager = "Set base:";
    ctrl = data.substring(manager.length());
    if (ctrl.startsWith("Milho")) {
      Serial.println();
      Serial.println("Milho");
      comp_humi = 24;
      comp_temp = 35;
      client.stop();
    } else if (ctrl.startsWith("Soja")) {
      Serial.println();
      Serial.println("Soja");
      comp_humi = 18;
      comp_temp = 35;
      client.stop();
    } else if (ctrl.startsWith("Feijao")) {
      Serial.println();
      Serial.println("Feijao");
      comp_humi = 18;
      comp_temp = 35;
      client.stop();
    }
  }
  if (data.startsWith("set EF=")) {
    manager = "set EF=";
    ctrl = data.substring(manager.length());
    if (ctrl.startsWith("ON") and EF == LOW) {
      EF = HIGH;
    } else {
      EF = LOW;
    }
    digitalWrite(fuso, EF);
    digitalWrite(vent, EF);
    client.stop();
  }

  if (data.startsWith("set manual=")) {
    manager = "set manual=";
    ctrl = data.substring(manager.length());
    int meio = ctrl.indexOf(",");
    comp_humi = ctrl.substring(0, meio).toFloat();
    Serial.print("humidade limite:");
    Serial.println(comp_humi);
    comp_temp = ctrl.substring(meio + 1).toFloat();
    Serial.print("temperatura limite:");
    Serial.println(comp_temp);
    client.stop();

  }
}
//Cria uma função para extrair as variaveis de rede
void net(String data) {
  int indexName = data.indexOf("name=") + 5;
  int indexEspaco = data.indexOf(" ", indexName);

  String Name_net = data.substring(indexName, indexEspaco);
  int indexSenha = data.indexOf(" Senha=") + 13 + Name_net.length();

  String pass_net = data.substring(indexSenha);
  Serial.println ("network connecting in:" + Name_net);
  Serial.println ("pass:" + pass_net);
  WiFi.disconnect();
  rede(Name_net, pass_net);//chama a função para conectar a rede
}
void Point() {
  Serial.println();
  Serial.print("Iniciando Ponto de Acesso ");
  Serial.println("Silo_AP");
  WiFi.softAP("Shadow", "Titanico");/*
  WiFi.softAPConfig(AP_IP, gateway_AP, subnet_AP);*/
  Serial.print("Endereço do Ponto:");
  Serial.println(WiFi.softAPIP());

}
//Cria um função de comando para conectar a rede
void rede(String ssid, String password) {
  delay(500);
  // Conecta-se à rede Wi-Fi
  Serial.println();
  Serial.print("Conectando-se à rede ");
  Serial.println(ssid);
  WiFi.config(NETWORK_IP, GATE, SUB);
  WiFi.begin(ssid, password);
  //Entra em loop ate estar conectado a rede, Se não tiver conectado após 5 segundos abrira o ponto de acesso novamente
  float startTime = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //Manda as Informações Ip para o monitor Serial
  IPAddress ip = WiFi.localIP();
  Serial.println("");
  Serial.println("Conectado à rede Wi-Fi");
  Serial.print("Endereço IP: ");
  Serial.println(ip);
}
void processSerialData(String data) {
  // Encontre os índices dos caracteres de início e fim de cada valor
  int humi1Start = data.indexOf("humi1=") + 6;
  int humi1End = data.indexOf(" ", humi1Start);

  int humi2Start = data.indexOf("humi2=") + 6;
  int humi2End = data.indexOf(" ", humi2Start);

  int temp1Start = data.indexOf("temp1=") + 6;
  int temp1End = data.indexOf(" ", temp1Start);

  int temp2Start = data.indexOf("temp2=") + 6;
  int temp2End = data.indexOf(" ", temp2Start);

  int gasStart = data.indexOf("gas=") + 4;
  int gasEnd = data.indexOf(" ", gasStart);


  // Extraia os valores usando a função substring()
  humi1 = data.substring(humi1Start, humi1End);
  humi2 = data.substring(humi2Start, humi2End);
  temp1 = data.substring(temp1Start, temp1End);
  temp2 = data.substring(temp2Start, temp2End);
  gas = data.substring(gasStart);
  //Calcula a media das medidas de temperatura e umidade
  humi = (humi1.toFloat() + humi2.toFloat()) / 2;
  temp = (temp1.toFloat() + temp2.toFloat()) / 2;
  //Executa a verificação da variavel e altera a String (para poder ser enviada aos paineis)
  if (EF == HIGH) {
    StrEF = "ON";
  } else {
    StrEF = "OFF";
  }
}
