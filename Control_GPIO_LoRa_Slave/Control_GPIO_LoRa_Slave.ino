#include <LoRaMESH.h>

// Definindo a comunicação com o LoRa via Serial1 (ou qualquer outra UART disponível no ESP32)
HardwareSerial mySerial(1);  // Serial1 ou outra UART disponível
LoRaMESH lora(&mySerial);

uint8_t ID = 1;

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  // Configurando a UART1 para comunicação com o módulo LoRa
  mySerial.begin(9600, SERIAL_8N1, 16, 17);  // UART1, RX no pino 16 e TX no pino 17 (ajuste conforme seu hardware)
  lora.begin(true);
  
  // Configure o pino de entrada (ajustado para o pino 4 do ESP32, mas pode ser modificado)
  pinMode(4, INPUT_PULLUP); // Alterar conforme necessidade
  
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
    }else Serial.println("bps OK");

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
  delay(1000);
  if(ID == 0 && digitalRead(4) == 0){
    static bool button_status = false;
    if(button_status)
      lora.digitalLoraWrite(1, LoRa_GPIO0, HIGH);
    else
      lora.digitalLoraWrite(1, LoRa_GPIO0, LOW);
    delay(1000);
  }

  const char *mensagem = "Olá, Master!";
    if (lora.PrepareFrameTransp(0, (uint8_t *)mensagem, strlen(mensagem))) {
      lora.sendPacket(); // Envia o pacote
      Serial.println("Mensagem enviada com sucesso!");
    } else {
      Serial.println("Falha ao preparar mensagem.");
    }
}

