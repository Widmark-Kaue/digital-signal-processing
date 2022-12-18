
int ValorRef, ValorSaida, AcaoPWM;
double Referencia, E, E1, E2, E3, E4, E5, S1, S2, S3, S4, S5, Y=0;
float deltaTempo, UltimaAcao;
float dt = 13.7931;


//double a0=0.6, a1=0, b1=0.1, b2=0.1, b3=0.2; 

float a[6] = {0.0011, 0.0056, 0.0112, 0.0112, 0.0056, 0.0011};
float b[6] = {1.0,   -3.0384, 3.9451, -2.671, 0.9342,-0.1342};

#define Ref       A0    // Leitura analógica da referência no pino A0
#define Atua      3     // Vai atuar no PWM do pino 3
#define medidaTempo    4     // 

void setup() {
  
  Serial.begin(9600);

  pinMode(Ref, INPUT);
  pinMode(Atua, OUTPUT);
  pinMode(medidaTempo, OUTPUT);

}


void Filtro(){

      // coloca pino em nível alto para avaliar quanto tempo demora para rodar esta rotina
      digitalWrite(medidaTempo,HIGH);

      // Instante desta ação
      UltimaAcao = millis();

      // Lê entrada
      ValorRef = analogRead(Ref);

      //Entrada ajustada para valor de 8 bits
      E = (map(ValorRef, 0, 1023, 0, 255));

      //Função de tranferência do filtro
      Y=E*a[0] + E1*a[1] + E2*a[2] + E3*a[3] + E4*a[4] + E5*a[5];
      Y= Y - (S1*b[1] + S2*b[2] + S3*b[3] + S4*b[4] +S5*b[5]);

      //limitadores de razão cíclica do PWM
      if (Y>254) {Y=255;}
      if (Y<1) {Y=0;}
      analogWrite(Atua, Y);

      // Guarda valores anteriores
      // E1=E;
      // S3=S2;
      // S2=S1;
      // S1=Y;
      E5=E4;
      E4=E3;
      E3=E2;
      E2=E1;
      E1=E;
      S5=S4;
      S4=S3;
      S3=S2;
      S2=S1;
      S1=Y;

      // coloca em nível baixo o pino para avaliar quanto tempo demora para rodar esta rotina
      digitalWrite(medidaTempo,LOW);
      
}


void loop() {


  deltaTempo = (millis() - UltimaAcao);                   // / 1000.0;

  if (deltaTempo>=dt) {Filtro();}
  
}
