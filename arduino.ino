int ledVermelho = 8;
int ledVerde = 6;
int ledAzul = 4;
int botao = 2; 

// TEMPO E ESTADO 
const unsigned long DESAFIO_DURACAO_MS = 50000; // 50 segundos
const unsigned long FEEDBACK_DURACAO_MS = 3000; 

unsigned long startTime = 0;
unsigned long feedbackStartTime = 0; 

bool desafioAtivo = false; 
bool desafioConcluido = false; 
bool sucesso = false; 

int virtualValue = 0; 


void setup() {
  Serial.begin(9600);
  
  pinMode(A0, INPUT);
  pinMode(botao, INPUT_PULLUP);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAzul, OUTPUT);
  
  // garante que tudo começa desligado
  digitalWrite(ledVermelho, LOW);
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAzul, LOW);
  
  Serial.println("Cronometro SilverTech pronto.");
}

void loop() {
  unsigned long currentTime = millis();
  int estadoBotao = digitalRead(botao);
  
  // BOTÃO
  if (estadoBotao == LOW) {
    delay(50); 
    if (digitalRead(botao) == LOW) {
      
      if (!desafioAtivo && !desafioConcluido) {
        // INÍCIO DO DESAFIO 
        startTime = currentTime; 
        desafioAtivo = true;
        sucesso = false; 
        
        Serial.println("DESAFIO INICIADO!");
        digitalWrite(ledAzul, HIGH); // liga o LED Azul para indicar inicio
        
      } else if (desafioAtivo && !desafioConcluido) {
        desafioAtivo = false; 
        desafioConcluido = true; 
        sucesso = true; 
        feedbackStartTime = currentTime;
        
        Serial.println("SUCESSO! Resposta correta.");
      }
      
      // Espera o botão ser solto
      while(digitalRead(botao) == LOW); 
    }
  }


  if (desafioAtivo) {
    unsigned long elapsedTime = currentTime - startTime;
    
    // mapeia o tempo decorrido para um valor de potenciometro (0 a 1023)
    virtualValue = map(elapsedTime, 0, DESAFIO_DURACAO_MS, 0, 1023);
    if (virtualValue > 1023) virtualValue = 1023;
    
    Serial.println("Tempo: " + (String)(elapsedTime / 1000) + "s | Valor Virtual: " + (String)virtualValue);

    
    // LED VERDE: 50% do tempo (Progressivo)
    if (virtualValue > 512) { 
      digitalWrite(ledVerde, HIGH);
    } else {
      digitalWrite(ledVerde, LOW);
    }
    
    // LED VERMELHO: 83% do tempo (progressivo)
    if (virtualValue > 850) { 
      digitalWrite(ledVermelho, HIGH);
    } else {
      digitalWrite(ledVermelho, LOW);
    }
    

    if (elapsedTime >= DESAFIO_DURACAO_MS) {
      
      Serial.println("TEMPO ESGOTADO! TENTE NOVAMENTE.");
      
      desafioAtivo = false; // para o cronômetro
      desafioConcluido = true; 
      sucesso = false; 
      feedbackStartTime = currentTime; 
    }
  }


  // (FEEDBACK FINAL E RESET)
  if (desafioConcluido) {
    
  
    digitalWrite(ledAzul, LOW); // desliga o azul
    if (sucesso) {
      digitalWrite(ledVerde, HIGH); // VERDE
      digitalWrite(ledVermelho, LOW);
    } else {
      digitalWrite(ledVermelho, HIGH); // VERMELHO
      digitalWrite(ledVerde, LOW);
    }

  
    if (currentTime - feedbackStartTime >= FEEDBACK_DURACAO_MS) {
      
      // 3. Reseta o sistema
      Serial.println("Desafio Reiniciado. Pressione o botão para iniciar.");
      digitalWrite(ledVermelho, LOW);
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledAzul, LOW);
      desafioConcluido = false; //  novo início
    }
  }

  delay(100); 
}
