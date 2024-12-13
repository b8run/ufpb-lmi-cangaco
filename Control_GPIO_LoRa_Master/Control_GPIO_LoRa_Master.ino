/*-------------------------------------------------------------------------------------
  Este código já faz uma autoconfiguração do módulo, então você não precisa
  fazer manualmente. Caso queria saber mais sobre esse módulo, acesse o link
  do nosso post.  

  Post completo : https://elcereza.com/lora-mesh-da-radioenge-tutorial-completo
  Autor : Gustavo Cereza
  Revisado e editado : Marco Chiodi (Diretor de P&D da Radioenge)
  Disponibilizado por : Elcereza
  Redes : @ellcereza
  Canal : https://t.me/elcereza
-------------------------------------------------------------------------------------*/

#include <LoRaMESH.h>

HardwareSerial mySerial(1);  // Serial1 ou outra UART disponível
LoRaMESH lora(&mySerial);

uint8_t ID = 0;

void setup() {
  Serial.begin(115200);
  delay(2000);
  mySerial.begin(9600, SERIAL_8N1, 16, 17);  // UART1, RX no pino 16 e TX no pino 17 (ajuste conforme seu hardware)
  lora.begin(true);
  pinMode(4, INPUT_PULLUP);

  if(lora.localId != ID)
  {
    if(!lora.setNetworkID(ID)){
      Serial.println("Erro ao definir o novo ID");
      while(1);
    }

    Serial.println("ID configurado com sucesso!");

    if(!lora.setBPS(BW500, SF7, CR4_5)){
      Serial.println("Erro ao configurar bps");
      while(1);
    }

    Serial.println("Parametros LoRa configurados com sucesso!");
    
    if(!lora.setClass(CLASS_C, WINDOW_5s)){
      Serial.println("Erro ao configurar a classe");
      while(1);
    }

    Serial.println("Modo de operacao configurado com sucesso!");

    if(!lora.setPassword(123)){
      Serial.println("Erro ao gravar a senha ou a senha gravada não condiz com a senha definida");
      while(1);
    }

    Serial.println("Senha configurada com sucesso!");
  }

  if(ID == 1)
    lora.pinMode(LoRa_GPIO0, LoRa_NOT_PULL, INOUT_DIGITAL_OUTPUT, LOW);

  Serial.println("LocalID: " + String(lora.localId));
  Serial.println("UniqueID: " + String(lora.localUniqueId));
  Serial.println("Pass <= 65535: " + String(lora.registered_password));
}

void loop() {

  if(ID == 0 && digitalRead(4) == 0 ){
    static bool button_status = false;
    if(button_status)
      lora.receivePacketTransp());
    else
      lora.digitalLoraRead(1, LoRa_GPIO0);
    delay(1000);
  }

}
