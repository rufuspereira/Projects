

/* DATE: 06-04-2016
************  This program sets the calendar, time and alarms ************/
//===== Libraries included ======================================
#include <Time.h>
#include <TimeAlarms.h>
#include <avr/wdt.h>
#include <TimerOne.h>
//=====  Variables ==============================================
int dia = 0;
int mes = 0;
int anyo = 0;
int hora = 0;
int minuto = 0;
int segundo = 0;
int alarmMinuto = 0;
int alarmHora = 0;
String DD;
String MM;
String AAAA;
String HH = "";
String MIN = "";
String SEG = "";
String input = "";
String hh = "";
String mm = "";
char c;
String instruct = "";
int primerNumero = 0;
String serialData = "";
unsigned long currentMillis;
unsigned long previousMillis = 0;
unsigned long intervalo = 3000;
String respuesta = "";

//=== function1 to print the command list:  ===========================
void printHelp1() {
  Serial.println ("Instrucciones para ajustes del calendario Rufus");
  Serial.println ("Calendario: Introduzca en este orden y con dos digitos");
  Serial.print ("dia: DD, mes: MM, anyo: AAAA\t");
  Serial.println ("Termine con un '/' ");
}
//=== function2 to print the command list:  ===========================
void printHelp2() {
  Serial.println ("Instrucciones para ajustes del reloj Rufus");
  Serial.print ("2. Hora: Introduzca la hora: HH y minutos: MM\t");
  Serial.println ("Termine con un '/' ");
}
//=== function3 to print the command list:  ===========================
void printHelp3() {
  Serial.println ("Instrucciones para ajustes de la alarma Rufus");
  Serial.print ("3. Hora: Introduzca la hora: HH y minutos: MM\t");
  Serial.println ("Termine con un '/' ");
}
//=== function to collect data throgh serial monitor:  ==================
void getEntry(String *devol1, int *devol2) {
  String salida = "";
  // Read any serial input
  while (Serial.available() > 0)
  {
    c = (char) Serial.read();
    Serial.print("input caracter "); Serial.println(c);
    input = input + c; // Read in one char at a time
    //Serial.print("inputagregado "); Serial.println(input);
    // Serial.print("longitud "); Serial.println(input.length());
    //Serial.print("Ultimo character "); Serial.println(input[input.length() - 1]);
    if (input[input.length() - 1] == '/') {
      salida = input.substring(0, input.length() - 1);
      Serial.print("input de salida "); Serial.println(salida);
      input = "";
      *devol1 = salida;
      *devol2 = salida[0] - '0';
      switch (*devol2) {
        case 1 :
          Serial.println("datos de fecha");
          break;
        case 2 :
          Serial.println("datos de hora");
          break;
        case 3 :
          Serial.println("datos de alarma");
          break;
      }
    }
  }
}
//=== function convert strings to nuneric :  ====================
void stringtoNumber(String instruct) {
  if (instruct.substring(0, 1) == "1") {
    DD = instruct.substring(1, 3);
    MM = instruct.substring(3, 5);
    AAAA = instruct.substring(5, 9);
    dia = DD.toInt();
    mes = MM.toInt();
    anyo = AAAA.toInt();

  }
  if (instruct.substring(0, 1) == "2") {
    HH = instruct.substring(1, 3);
    MIN = instruct.substring(3, 5);
    SEG = instruct.substring(5, 7 );
    hora = HH.toInt();
    minuto = MIN.toInt();
    segundo = SEG.toInt();
  }
  if (instruct.substring(0, 1) == "3") {
    hh = instruct.substring(1, 3);
    mm = instruct.substring(3, 5);
    alarmHora = hh.toInt();
    alarmMinuto = mm.toInt();
  }
}
//=== function alarm Service : ================================================
//A) Nº de los puertos conectados a los pines.
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
const int buzzPin = 3;

//B) Las intensidades de cada color dadas por el ciclo de trabajo del PWM

const int redInt = 254;
const int greenInt = 254;
const int blueInt = 255;

//E) la variable volátil para intercambiar;
volatile int flag;
int k=0;
  //Apago todos los colores
  
  //Calling functions
 //Function callback()
void callback()
{
  k=k+1;
  flag=k%6;
  //0: red
  //1:green
  //2:blue
   
}
//
void Alarma(){
switch (flag) {
    case 0:
      analogWrite(redPin, redInt);
      analogWrite(buzzPin, 0);
      break;
    case 1:
      analogWrite(redPin, 0);
      analogWrite(greenPin, 0);
      analogWrite(bluePin, 0);
       analogWrite(buzzPin, 255);
    break;
    case 2:
      analogWrite(greenPin, greenInt);
      analogWrite(buzzPin, 0);
      break;
     case 3:
      analogWrite(redPin, 0);
      analogWrite(greenPin, 0);
      analogWrite(bluePin, 0);
      analogWrite(buzzPin, 127);
     break;
     case 4:
      analogWrite(bluePin, blueInt);
      analogWrite(buzzPin, 0);
      break;
     case 5:
      analogWrite(redPin, 0);
      analogWrite(greenPin, 0);
      analogWrite(bluePin, 0);
      analogWrite(buzzPin, 65);
      
      break;
    default: 
      analogWrite(bluePin, blueInt^1);
    break;
  }
}

//---------------- setup ---------------------------------------------
void setup() {

  while (!Serial);
  setTime(0, 0, 0, 1, 1, 1970);
  wdt_disable(); // Desactivar el watchdog
  Serial.begin(9600);   // Open serial port (9600 bauds).
  printHelp1();          // Print the command list.
  Serial.println ("comienzo el esto ");
  Serial.print ("fecha inicial "); Serial.printf ("%d-%d-%d.\n", day(), month(), year());
  printHelp2();
  Serial.print ("hora inicial "); Serial.printf ("%d:%d:%d.\n", hour(), minute(), second());
  printHelp3();
  Serial.print ("alarma inicial "); Serial.printf ("%d:%d:%d.\n", 0, 0, 0);
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  Timer1.initialize(2000000);         // initialize timer1, and set a 1 second period
  Timer1.attachInterrupt(callback); 
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
  //indico que están apagados
  String redState = "LOW";
  String greenState = "LOW";
  String blueState = "LOW";

}
//---------------- loop ---------------------------------------------

void loop() {
  currentMillis = millis();
  while ((Serial.available()) && (dia == 0 && mes == 0 && anyo == 0) || (hora == 0 && minuto == 0 && segundo == 0) || (alarmHora == 0 && alarmMinuto == 0)) {
    getEntry(&serialData, &primerNumero);
    stringtoNumber(serialData);
    setTime(hora, minuto, segundo, dia, mes, anyo);
  }
 /* Alarm.alarmRepeat(dowMonday, alarmHora, alarmMinuto, 10, Alarma);
  Alarm.alarmRepeat(dowTuesday, alarmHora, alarmMinuto, 10, Alarma);
  Alarm.alarmRepeat(dowWednesday, alarmHora, alarmMinuto, 10, Alarma);
  Alarm.alarmRepeat(dowThursday, alarmHora, alarmMinuto, 10, Alarma);
  Alarm.alarmRepeat(dowFriday, alarmHora, alarmMinuto, 10, Alarma);*/
  if (currentMillis - previousMillis >= intervalo) {

    previousMillis = currentMillis;
    Serial.print ("\nserialData\t"); Serial.println (serialData);
    Serial.printf ("Fecha: %d-%d-%d. Hora: %d:%d:%d\n", day(), month(), year(), hour(), minute(), second());
    Serial.printf ("Alarma: %d:%d\n", alarmHora, alarmMinuto);
  }
  else {
    Serial.print("");
  }
  Alarm.delay(0);
}
