#include <Ultrasonic.h>
#include <LiquidCrystal.h>


int outRelay = 53;
int alarmaAgua = 52;
int alarmaPresion = 51;
int puerta = 50;
int alarmaPuerta = 49;
int presionCamara = 48;
int alarmaCamara = 47;
int alarmaFinDeCiclo = 46;
int banderaI= 45;
int trigger=36;
int echo=37;
int ThresholdLowPre=15;
int ThresholdHighPre=25;
LiquidCrystal lcd(23, 24, 25, 26, 27, 28);
LiquidCrystal lcd2(29, 30, 31, 32, 33, 34);

Ultrasonic ultrasonic(trigger,echo);

void setup()
{

  //Nivel agua
  pinMode(outRelay,OUTPUT);
  pinMode(alarmaAgua,OUTPUT);
  pinMode(alarmaPresion,OUTPUT);
  pinMode(puerta,INPUT);
  pinMode(alarmaPuerta,OUTPUT);
  pinMode(presionCamara,INPUT);
  pinMode(banderaI,INPUT);
  pinMode(alarmaCamara,OUTPUT);
  pinMode(alarmaFinDeCiclo,OUTPUT);
  lcd.begin(16, 2);
  lcd2.begin(16, 2);
  Serial.begin(9600); 
}

void loop()
{
  int banderaInicio=digitalRead(banderaI);
          lcd.setCursor(0, 0);
          lcd.print("PRESIONA BOTON");
          lcd.setCursor(0, 1);
          lcd.print("PARA DAR INICIO");
  while(banderaInicio==1)
  {
      double nivelagua=ultrasonic.Ranging(CM);
      //Impresion en monitor serial de arduino del nivel de agua
      Serial.print("Nivel de agua: ");
      Serial.println(nivelagua);
      
      if (nivelagua<=23){
        digitalWrite(alarmaAgua,LOW);
        float temperatura = analogRead(0);
        double presion = analogRead(1);
        //Conversion de senal analoga obtenida por sensor de presion
        presion = presion * (92.6 / 1023);
        delay(50);
        //Conversion de senal analoga obtenida por sensor de temperatura
        temperatura = (5 * temperatura * 100)/1023;
          
         //Impresion en monitor serial de las medidas obtenidas por sensores
         Serial.print("T: ");
         Serial.println(temperatura);
         Serial.print("P: ");
         Serial.println(presion);
        
        //Evaluacion temperatura se encuentra en rango
        if(temperatura<=40)
        {
          //Encendido de resistencia calefactora
          digitalWrite(outRelay,HIGH);
        }
        if(temperatura>=46)
        {
          //Apagado de resistencia calefactora
          digitalWrite(outRelay,LOW);
        }
        if(temperatura <=40 || temperatura>=45)
        {
          digitalWrite(alarmaPresion,LOW);
          digitalWrite(alarmaPuerta,LOW);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("TEM. NO ESTABLE");
          lcd.setCursor(0, 1);
          lcd.print("ETAPA POTENCIA");
          delay(1500);
        }
        if(presion<=ThresholdLowPre)
        {
          //Display Presion baja
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("ALERTA PB!");
          lcd.setCursor(0, 1);
          lcd.print("ETAPA POTENCIA");
           digitalWrite(alarmaPuerta,LOW);
          //Alarma 2
          digitalWrite(alarmaPresion,HIGH);
          delay(1500);
        }
        if(presion>=ThresholdHighPre)
        {
          //Display Presion ALTA
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("ALERTA PA!");
          lcd.setCursor(0, 1);
          lcd.print("ETAPA POTENCIA");
          digitalWrite(alarmaPuerta,LOW);
          //Alarma 2
          digitalWrite(alarmaPresion,HIGH);
          delay(1500);
        }
        
        if(temperatura>=40 && temperatura<=45 && presion>=ThresholdLowPre && presion<=ThresholdHighPre)
        {
          digitalWrite(alarmaPresion,LOW);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("E. POTENCIA END");
          lcd.setCursor(0, 1);
          lcd.print("VALORES VERIFICA");
          delay(1500);
          lcd.clear();
          //Lectura de estado de la puerta (abierta/cerrada)
          int StatusPuerta = digitalRead(puerta);
          if(StatusPuerta==1)
          {
            digitalWrite(outRelay,LOW); //apagar resistencia
            digitalWrite(alarmaPuerta,LOW);
            lcd.setCursor(0, 0);
            lcd.print("PUERTA CERRADA");
            lcd.setCursor(0, 1);
            lcd.print("INICIANDO PROCES");
              delay(4000);
              lcd.clear();
              //Pre-vacio
              //Display (Prevacio)
              lcd.setCursor(0, 1);
              lcd.print("ETAPA PRE-VACIO");
              delay(3000); //Pre-vacio
              lcd.clear();
              //Pre-camara
              //Display(precamara)
              lcd.setCursor(0, 1);
              lcd.print("ETAPA PRE-CAMARA");
              delay(3000); //Entrada vapor en camara
              lcd.clear();
              //Valvula pre-camara/camara
              //Display(Valvula pre-camara/camara)
              lcd.setCursor(0, 1);
              lcd.print("ETAPA PRE-CAM/CA");
              delay(3000);
              lcd.clear();
              int bandera=1;
              while(bandera==1)
              {
                //Lectura del estado de presion en camara (estable/no estable)
                int igualdadPresion=digitalRead(presionCamara);
                if(igualdadPresion==1)
                {
                  digitalWrite(alarmaCamara,LOW);
                  //Display(entrada de vapor)
                  lcd.setCursor(0, 0);
                  lcd.print("ESTERILIZACION");
                  lcd.setCursor(0, 1);
                  lcd.print("ETAPA IN VAPOR");
                  delay(3000);
                  lcd.clear();
                  //Display(fin de ciclo)
                  lcd.setCursor(0, 1);
                  lcd.print("FIN DE CICLO");
                  //alarma 5
                  digitalWrite(alarmaFinDeCiclo,HIGH);
                  delay(5000);
                  digitalWrite(alarmaFinDeCiclo,LOW);
                  bandera=0;
                  banderaInicio=0;
                  lcd.clear();
                  lcd.setCursor(0, 1);
                  lcd.print("ETAPA PRE-CAM/CA");
                  delay(3000);
                  lcd.clear();
                  lcd.setCursor(0, 1);
                  lcd.print("ETAPA PRE-VACIO");
                  delay(3000);
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("PROCESO EXITOSO");
                  lcd.setCursor(0, 1);
                  lcd.print("      :D");
                  delay(3000);
                  lcd.clear();
                  //Fin de todo el proceso (regresa al inicio)
                }
                else
                {
                  //Alarma 4
                  //Display (presion no estable)
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("ALERTA P-N-E");
                  digitalWrite(alarmaCamara,HIGH);
                  delay(500);
                }
              }   
          }
          else
          {
            //Alarma 3
            //Display(puerta abierta)
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("ALERTA PTA.AB");
            digitalWrite(alarmaPuerta,HIGH);
            delay(1500);
            lcd.clear();
          }
        }
      }
      else
      {
        lcd.clear();
        //Apagado de resistencia calefactora
        digitalWrite(outRelay,LOW);
        digitalWrite(alarmaPuerta,LOW);
        //Diplay No hay agua (alarma 1)
        lcd.setCursor(0, 0);
        lcd.print("ALERTA AGUA INSU");
        lcd.setCursor(0, 1);
        lcd.print("ETAPA POTENCIA");
        //Alarma 1
        digitalWrite(alarmaAgua,HIGH);
        delay(500);
      }
    }
}
