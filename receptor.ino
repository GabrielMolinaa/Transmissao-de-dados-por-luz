/* Projeto Comunicação de Dados

GABRIEL MOLINA DE LIMA
GUILHERME DE ALMEIDA DO CARMO
GUSTAVO GEOVANE TAMIÃO DE SOUZA
PEDRO MIAN PARRA
 
 Código para Receber a mensagem, decodifica-la e exibi-la no terminal.

*/



// Declaração das váriaveis
int valor; 
int k = 0; 
int estado = 0;
int TaxaTransferencia = 500;
long divisor = 65521; //Divisor utilizado para o cálculo do CRC

char c;
char crc[16]; 
char anterior = '1';
char mensagem[256];
char binFinal[272]; 
char auxChar[9]; 
char binDivisor[17]; 

//  =-=-= Escolha do método de envio =-=-=
//          0  Para NRZ-Level 
//          1  Para NRZ-Invert
            
            int metodo = 1; 

// -----------------------------------------

// Função auxiliar do cáclculo do CRC
char exor(char a,char b){ 
  if(a==b)
    return '0';
  else
    return '1';
}// Funcao exor

// Função para calcular o CRC
void checkcrc(char data[], char key[]){ 

    int datalen = strlen(data);
    int keylen = strlen(key);
    char temp[17]; 
    char rem[17]; 

    // É preciso adicionar o 0 em toda a data restante
    for (int i = 0; i < keylen - 1; i++) {
        data[datalen + i] = '0';
    }//for

    data[datalen + keylen - 1] = '\0';

    // Para determinar o tamanho da palavra
    int codelen = datalen + keylen - 1; 

    for (int i = 0; i < keylen; i++){
        rem[i] = data[i]; 
    }// for

    for (int j = keylen; j <= codelen; j++){
        for (int i = 0; i < keylen; i++){
            temp[i] = rem[i]; 
        }// for

        if (rem[0] == '0'){
            for (int i = 0; i < keylen - 1; i++){
                rem[i] = temp[i + 1];
            }// for
        }else{
            for (int i = 0; i < keylen - 1; i++){
                rem[i] = exor(temp[i + 1], key[i + 1]);
            }// for
        }// if else
        
        if (j != codelen){
            rem[keylen - 1] = data[j]; 
        }else{
            rem[keylen - 1] = '\0';
        }// if else
    }//for

    for (int i = 0; i < keylen - 1; i++){
        data[datalen + i] = rem[i];
    }// for
    
    data[codelen] = '\0';

    Serial.print("O resto da divisao: ");
    Serial.println(rem);
    
 
}// Função checkcrc

void setup() {
  
  Serial.begin(9600);
  delay(1000);

  //Converção do numero divisor para binario
  itoa(divisor, binDivisor, 2);  
  
}// Função setup


void loop() {
  
  if (estado  = 0){
    Serial.println("Receptor iniciado. Aguardando inicio do envio");
    delay(1000);
    estado  = 1;
  }
  

  Serial.print("Luminosidade: ");
  valor = analogRead(A1);
  Serial.println(valor);
  
  // Receptor aguarda uma emissão de luz para iniciar a sicronização com o transmissor
  if (valor > 300){
    
    Serial.println("Em 5 segundos comecara o recebimento da mensagem");
    delay(5000);
    Serial.println("Iniciando o recebimento da mensagem");

    for (int i = 0; i < 256; i++) {
     
      valor = analogRead(A1);

      // Caso receba uma 'meia luz', significa que começara o envio do CRC
      if (valor > 400 && valor < 650){
        
        Serial.println("Mensagem Recebida!");
        Serial.println("Comecando recebimento do CRC");
        
        delay(5000);

        for (int l = i; l < i+15; l++) {
          
          valor = analogRead(A1);

        // Caso LED aceso
          if (valor > 400){

                switch (metodo){
                // NRZ-Level
                case 0:
                    crc[k] = '1';
                    binFinal[l] = '0';            
                    break;
                
                // NRZ-Invert
                case 1:

                crc[k] = '1';

                if(crc[k]==anterior){
                binFinal[l] = '0';
                }else{
                binFinal[l] = '1';
                }//if else 
                anterior = crc[k];
                    break;

                default:
                    Serial.println("Metodo de envio invalido!");
                    break;
                }// switch
            
        // Caso LED apagado 
          }else{
            switch (metodo){
                // NRZ-Level
                case 0:
                   crc[k] = '0';
                   binFinal[l] = '1';            
                    break;
                
                // NRZ-Invert
                case 1:
                    crc[k] = '0';
                    
                    if(crc[k]==anterior){
                    binFinal[l] = '0';
                    }else{
                    binFinal[l] = '1';
                    } 
                    anterior = crc[k];
                    
                    break;

                default:
                    Serial.println("Metodo de envio invalido!");
                    break;
                }// switch
       
          }// if else estado do LED

          Serial.print(crc[k]);
          Serial.print(" ");

          k++;
      
          delay(TaxaTransferencia);
        }// for
        break;
      }// receber CRC

    // Recebimento da mensagem

       // LED acesso
        if (valor > 400){
             switch (metodo){
                // NRZ-Level
                case 0:
                    mensagem[i] = '1';
                     binFinal[i] = '0';             
                    break;
                
                // NRZ-Invert
                case 1:
                  mensagem[i] = '1';
                    
                    if(mensagem[i]==anterior){
                    binFinal[i] = '0';
                    }else{
                    binFinal[i] = '1';
                    } 
                   anterior = mensagem[i];
                            
                    break;

                default:
                    Serial.println("Metodo de envio invalido!");
                    break;
               }// switch

        // LED apagado
        }else{
            switch (metodo){
                // NRZ-Level
                case 0:
                     mensagem[i] = '0';
                     binFinal[i] = '1';               
                    break;
                
                // NRZ-Invert
                case 1:
                   mensagem[i] = '0';
            
                    if(mensagem[i]==anterior){
                         binFinal[i] = '0';
                    }else{
                          binFinal[i] = '1';
                    } // if else
                    anterior = mensagem[i];
                    break;

                default:
                    Serial.println("Metodo de envio invalido!");
                    break;
               }// switch

        }// Estado do led
              
      Serial.print(mensagem[i]);
      Serial.print(" ");

      delay(TaxaTransferencia);
      
    }

    Serial.print("Mensagem:");
    Serial.println(mensagem);
    Serial.print("CRC: ");
    Serial.println(crc);


    Serial.print("Mensagem Final recebida: ");
    for(int i = 0; i < (strlen(binFinal))-16;i+=8){ 
      for(int p = 0; p < 8;p++){
        auxChar[p] = binFinal[i+p];
      }
      c = strtol(auxChar, 0, 2);
      
      Serial.print(c);
      
    }

    Serial.println(" ");
    checkcrc(binFinal,binDivisor);

    delay(3000);

    exit(1);
    
  } 

  
}