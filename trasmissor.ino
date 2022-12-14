/* Projeto Comunicação de Dados

GABRIEL MOLINA DE LIMA
GUILHERME DE ALMEIDA DO CARMO
GUSTAVO GEOVANE TAMIÃO DE SOUZA
PEDRO MIAN PARRA
 
 Código para codificar e enviar a mensagem.

*/

int estado = 0; 
int TaxaTransferencia = 500;
long divisor = 65521; 
char aux = '1';
char binMensagem[256]; 
char binCaracter[8]; 
char binDivisor[17]; 
char binFinal[272]; 


//  =-=-= Mensagem para ser enviada =-=-=
      
           String Mensagem = "Mensgaem Teste";

// -----------------------------------------

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
void crc(char data[], char key[]){   

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
    }//for

    for (int j = keylen; j <= codelen; j++){
        for (int i = 0; i < keylen; i++){
            temp[i] = rem[i];
        }//for

        if (rem[0] == '0'){
            for (int i = 0; i < keylen - 1; i++){
                rem[i] = temp[i + 1];
            }//for
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
    }// for

    for (int i = 0; i < keylen - 1; i++){
        data[datalen + i] = rem[i];
    }// for

    data[codelen] = '\0';

    strcpy(binFinal, data);
    
    Serial.println("CRC para envio: ");
    Serial.println(rem);
    Serial.println("Data para envio:");
    Serial.println(data);

}


void setup() {
  
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
  Serial.begin(9600);
  
  char Caracter;

  delay(1000);

  for (int i = 0; i < Mensagem.length(); i++) {
    // Transformar caracter por caracter para binario 
    Caracter = Mensagem.charAt(i); 
    itoa(Caracter, binCaracter, 2); 
    binMensagem[strlen(binMensagem)] = '0';
    // Tratamento para espaço
    if (Caracter == ' '){ 
      binMensagem[strlen(binMensagem)] = '0';
    }
    strcat(binMensagem, binCaracter);
  }

  Serial.print("Mensagem: "); 
  Serial.println(Mensagem);

  
  Serial.print("Codigo binario: ");
  Serial.println(binMensagem);


  
  itoa(divisor, binDivisor, 2); 
 
  crc(binMensagem,binDivisor);



    switch (metodo){
                // NRZ-Level
                case 0:
                   for (int i = 0; i < (strlen(binFinal))+1; i++){
                        if(binFinal[i]== '1')  {
                             binFinal[i] = '0';
                        }else if (binFinal[i]== '0')  {
                             binFinal[i] = '1';
                        }// eske if
                    }// for          
                    break;
                
                // NRZ-Invert
                case 1:
                      for (int i = 0; i < (strlen(binFinal))+1; i++){
                        if(binFinal[i]== '1')  {
                            if(aux=='1'){
                                aux='0';
                            }else{
                                aux='1';
                            }//if else
                         binFinal[i] = aux;
                        }else if (binFinal[i]== '0')  {
                           binFinal[i] = aux;
                        }// if else
                    }//for
                    break;
                }// switch
}//setup


void loop() {

  if (estado == 0){
    Serial.println("Transmissor iniciando");
    delay (5000);
    estado = 1;
  }//if

  if (estado = 1){

    int contador1 = Mensagem.length() * 8;
    
    digitalWrite(9, HIGH); 
    Serial.println("Envio comecara em 5 segundos");
    delay(4000);
    digitalWrite(9, LOW);
    delay(1000);
    
    Serial.println("Envio iniciado!");

    for (int i = 0; i < contador1; i++) { 
        Serial.print(binFinal[i]);
        Serial.print(" ");

        if (binFinal[i] == '1'){
          digitalWrite(9, HIGH);
        }else{
          digitalWrite(9, LOW);
        }//if else

      delay(TaxaTransferencia);
      
    }//for

    // Para avisar o fim do envio da mensagem, emite uma 'meia-luz'
    analogWrite(9,3); 
    Serial.println("Mensagem enviada!");

    Serial.println("Iniciando envio do CRC");
    
    delay(5000);
    
    analogWrite(9,0);

    for (int i = contador1; i < contador1+17; i++) { 
      
      Serial.print(binFinal[i]);
      Serial.print(" ");

      if (binFinal[i] == '1'){
        digitalWrite(9, HIGH);
      }else{
        digitalWrite(9, LOW);
      }// if else



      delay(TaxaTransferencia);
      
    }// for
    
    // Para avisar o fim do envio do CRC, emite uma 'meia-luz' novamente
    analogWrite(9,3); 


   
    Serial.println("Dados transmitidos");
    Serial.println("Encerrando...");

    delay(3000);

    exit(1);
    
 }// if
            
}// loop