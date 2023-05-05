#include <PCD8544.h> //Incluyo libreria PCD8544
#include <math.h>
#include <stdio.h>


// Declaracion de Hardware y variables necesarias
static PCD8544 lcd;
//LEDS para activar alarmas
int l_led1 = 12; // LED canal 1
int l_led2 = 13; // LED canal 2
int l_led3 = 18; // LED canal 3
int l_led4 = 19; // LED canal 4

int a_input3 = 3; //Entrada analogica canal 1
int a_input2 = 2; //Entrada analogica canal 2
int a_input1 = 1; //Entrada analogica canal 3
int a_input0 = 0; //Entrada analogica canal 4

// Variables para el circuito
float R1 = 25000.0;  //  R1 (25K) Division de tension
float R2 = 5000.0; //  R2 (5k)    Division de tension


// Variables analogicas
int v_analog3 = 0; //Variable analogica canal 1 
int v_analog2 = 0; //Variable analogica canal 2 
int v_analog1 = 0; //Variable analogica canal 3  
int v_analog0 = 0; //Variable analogica canal 4

//Entradas digitales
byte d_input8 = 8;      // Canal 1
byte d_input9 = 9;      // Canal 2
byte d_input10 = 10;    // Canal 3
byte d_input11 = 11;    // Canal 4

//Entrada positiva o negativa

byte Posi_Nega8 = 0;   //Byte selector positivo o negativo canal 1, pin digital 8
byte Posi_Nega9 = 0;   //Byte selector positivo o negativo canal 2, pin digital 9
byte Posi_Nega10 = 0;  //Byte selector positivo o negativo canal 3, pin digital 10
byte Posi_Nega11 = 0;  //Byte selector positivo o negativo canal 4, pin digital 11

//Variables Vrms

float v_rms3 = 0; //Canal 1
float v_rms2 = 0; //Canal 2
float v_rms1 = 0; //Canal 3
float v_rms0 = 0; //Canal 4

//Variables Vout
float v_out3 = 0; //Canal 1
float v_out2 = 0; //Canal 2
float v_out1 = 0; //Canal 3
float v_out0 = 0; //Canal 4

//Varaibles Vin
float v_in3 = 0; //Canal 1
float v_in2 = 0; //Canal 2
float v_in1 = 0; //Canal 3
float v_in0 = 0; //Canal 4

//float v_out0 = 0.0;   // Definimos la variable Vout
//float v_in0 = 0.0;    // Definimos la variable Vin
///////////////////////////////////////////////////////////////////////////////////////////////////////
//Esta invertido, canal 4 =3, canal 2 =2, canal 1 =3 y canal 0 = 1.
void setup()
{
 
 
  // Declaracion de puertos de entrada
  pinMode(a_input3, INPUT); // Activamos el input del pin analógico A3
  pinMode(a_input2, INPUT); // Activamos el input del pin analógico A2
  pinMode(a_input1, INPUT); // Activamos el input del pin analógico A1
  pinMode(a_input0, INPUT); // Activamos el input del pin analógico A0

  // Declaracion de puertos de entrada para trabajar en modo digital    
  pinMode(d_input8, INPUT);  // Pin digital 8 como entrada
  pinMode(d_input9, INPUT);  // Pin digital 9 como entrada
  pinMode(d_input10, INPUT); // Pin digital 10 como entrada
  pinMode(d_input11, INPUT); // Pin digital 11 como entrada

  // Ajustes del display
  lcd.begin(84, 48);       // LCD ON
  lcd.print("Voltimetro");
  lcd.setCursor(15, 1);    // LCD
  lcd.print("Canales 4");
  
  //Pines LED como salidas
  pinMode(l_led1, OUTPUT); 
  pinMode(l_led2, OUTPUT);
  pinMode(l_led3, OUTPUT);
  pinMode(l_led4, OUTPUT);

  Serial.begin(9600); //Velocidad a la que se establece la omunicacion serial, en python tambien va 9600.
  pinMode(2, INPUT);  //Entrada para el swirch AC/DC 

}
///////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  
//Canal 1
  Posi_Nega8 = digitalRead(d_input8); //Selector tension positiva o negativa
  bool estadoPulsador3 = digitalRead(2); //Selector AC/DC
 
  v_analog3 = analogRead(a_input3); //Valor de tension (0V-5V) del pin analogico A3. 
  v_out3 = (v_analog3* 5.0145) / 1024; 
  v_in3 = v_out3 / (R2 / (R1 + R2));  //Division de tension 

  if (estadoPulsador3 == LOW)         //Valor bajo, tension DC    
  {
    if (Posi_Nega8 == LOW)            //Valor bajo, tension DC positiva  
    {                    
      if (v_in3 > 24)                 //Si el valor de entrada es mayor a 24, se enciendde el l_led1(Pin 12), de lo contrario el led se mantiene apagado
      {                               
        digitalWrite(l_led1, HIGH);
      }
      else
      {
        digitalWrite(l_led1, LOW);
      }
      
      lcd.setCursor(12, 2);         //Valor de tension se muestra en la pantalla LCD 
      lcd.print("V1 = ");   
      lcd.print(v_in3); 
      
      Serial.print("V1 ");         //Se envia el dato de la tension DC positiva
      Serial.println(v_in3);
      delay(400);
    }
    else
    {
      if (Posi_Nega8 == HIGH) //Valor alto, tension positiva
      {                    
        if (v_in3 * -1 < -24) //Si el valor de entrada es menor a -24, se enciendde el l_led1(Pin 12), de lo contrario el led se mantiene apagado
        {
          digitalWrite(l_led1,HIGH);
        }
        else
        {
          digitalWrite(l_led1, LOW);
        }
        
        lcd.setCursor(12, 2); //Valor de la tension negativa se muestra en la pantalla LCD      
        lcd.print("V1 ");     
        lcd.print(v_in3 * -1); 
        
        Serial.print("V1 "); //Se envia el dato de la tension DC negativa
        Serial.println(v_in3 * -1);
        delay(400);
      }
    }
  }

  if (estadoPulsador3 == HIGH) //Valor alto, tension AC 
  {
   
    if (v_in3 > v_rms3)       //Se toma el valor mas alto de la tension, posteriormente se multiplica por 0.707 para obtener Vrms
    {
      v_rms3 = v_in3;
      if (v_rms3*0.707 > 17) //Llevamos a cabo la multiplicacion 24*0.707 = 16.96, asi nos damos cuenta si sobrepaso el limite de 24V.
      {
        digitalWrite(l_led1, HIGH); //Si es mayor a 17, encedemos el led de lo contrario se mantiene apagado.
      }
      else
      {
        digitalWrite(l_led1, LOW);
      }
     
    }
    
      lcd.setCursor(12, 2);    //Valor de la tension Vrms se muestra en la pantalla      
      lcd.print("V1 RMS:");            
      lcd.print(v_rms3 *0.7071); 

      //Conexion serial tension AC
      Serial.print("Vrms1 ");
      Serial.println(v_rms3 *0.707);
      delay(400);
    }   


// CANAL 2

  Posi_Nega9 = digitalRead(d_input9); //Selector tension positiva o negativa
  bool estadoPulsador2 = digitalRead(2); //Selector AC/DC 
  
  v_analog2 = analogRead(a_input2); //Valor de tension (0V-5V) del pin analogico A2.  
  v_out2 = (v_analog2* 5.0145) / 1024; 
  v_in2 = v_out2 / (R2 / (R1 + R2)); //Division de tension

  if (estadoPulsador2 == LOW)  //Valor bajo, tension DC        
  {
    if (Posi_Nega9 == LOW) //Valor bajo, tension DC positiva
    {                   
      if (v_in2 > 24) //Si el valor de entrada es mayor a 24, se enciendde el l_led1(Pin 12), de lo contrario el led se mantiene 
      {
        digitalWrite(l_led2, HIGH);
      }
      else
      {
        digitalWrite(l_led2, LOW);
      }
      
      lcd.setCursor(12, 3); //Valor de tension se muestra en la pantalla LCD 
      lcd.print("V2= ");   
      lcd.print(v_in2); 
      
      Serial.print("V2 ");//Valor de tension se muestra en la pantalla LCD 
      Serial.println(v_in2);
      delay(400);
    }
    else
    {
      if (Posi_Nega9 == HIGH)  //Valor alto, tension positiva
      {                  
        if (v_in2*-1 < -24)//Si el valor de entrada es menor a -24, se enciendde el l_led1(Pin 12), de lo contrario el led se mantiene apagado
        {
          digitalWrite(l_led2, HIGH);
        }
        else
        {
          digitalWrite(l_led2, LOW);
        }
       
        lcd.setCursor(12, 3);  
        lcd.print("V2 ");     
        lcd.print(v_in2 * -1); 
        
        Serial.print("V2 ");
        Serial.println(v_in2 * -1);
        delay(400);
      }
    }
  }

  if (estadoPulsador2 == HIGH) 
  {
    //Voltaje mas alto
    if (v_in2 > v_rms2)
    {
      v_rms2 = v_in2;
      if (v_rms2*0.7071 > 17)
      {
        digitalWrite(l_led2, HIGH);
      }
      else
      {
        digitalWrite(l_led2, LOW);
      }
      
      lcd.setCursor(12, 3);         
      lcd.print("V2 RMS:");              
      lcd.print(v_rms2 *0.7071); 

      Serial.print("Vrms2 ");
      Serial.println(v_rms2*0.707);
      delay(400);
    }
  }



// Canal 3
  
  Posi_Nega10 = digitalRead(d_input10); //Selector tension positiva o negativa
  bool estadoPulsador1 = digitalRead(2); //Selector AC/DC
 
  v_analog1 = analogRead(a_input1); //Valor de tension (0V-5V) del pin analogico A1. 
  v_out1 = (v_analog1 * 5.0145) / 1024; 
  v_in1 = v_out1 / (R2 / (R1 + R2)); 

  if (estadoPulsador1 == LOW)        
  {
    if (Posi_Nega10 == LOW) 
    {                   
      if (v_in1  > 24)
      {
        digitalWrite(l_led3, HIGH);
      }
      else
      {
        digitalWrite(l_led3, LOW);
      }
      
      lcd.setCursor(12, 4); 
      lcd.print("V3= ");   
      lcd.print(v_in1); 
     
      Serial.print("V3 ");
      Serial.println(v_in1);
      delay(400);
    }
    else
    {
      if (Posi_Nega10 == HIGH) 
      {                    
        if (v_in1 * -1 < -24)
        {
          digitalWrite(l_led3, HIGH);
        }
        else
        {
          digitalWrite(l_led3, LOW);
        }
       
        lcd.setCursor(12, 4);  
        lcd.print("V3 ");    
        lcd.print(v_in1 * -1); 
        
        Serial.print("V3 ");
        Serial.println(v_in1 * -1);
        delay(400);
      }
    }
  }

  if (estadoPulsador1 == HIGH) 
  {
    // Para enocntrar el voltaje mas alto de la senal sinusoidal
    if (v_in1 > v_rms1)
    {
      v_rms1 = v_in1;
      if (v_rms1*0.7071 > 17)
      {
        digitalWrite(l_led3, HIGH);
      }
      else
      {
        digitalWrite(l_led3, LOW);
      }
      
      lcd.setCursor(12, 4);           
      lcd.print("V3 RMS:");              
      lcd.print(v_rms1 *0.7071); 
      
      Serial.print("Vrms3 ");
      Serial.println(v_rms1*0.707);
      delay(400);
    }
  }



  // Canal 4
  Posi_Nega11 = digitalRead(d_input11); //Selector tension positiva o negativa 
  bool estadoPulsador0 = digitalRead(2); //Selector AC/DC
  
  v_analog0 = analogRead(a_input0);  //Valor de tension (0V-5V) del pin analogico A0. 
  v_out0 = (v_analog0 * 5.0145) / 1024; 
  v_in0 = v_out0 / (R2 / (R1 + R2));  

  if (estadoPulsador0 == LOW)        
  {
    if (Posi_Nega11 == LOW) 
    {                   
      if (v_in0  > 24)
      {
        digitalWrite(l_led4, HIGH);
      }
      else
      {
        digitalWrite(l_led4, LOW);
      }
     
      lcd.setCursor(12, 5); 
      lcd.print("V4= ");   
      lcd.print(v_in0);
    
      Serial.print("V4 ");
      Serial.print(v_in0);
      delay(400);
    }
    else
    {
      if (Posi_Nega11 == HIGH) 
      {                   
        if (v_in0 * -1 < -24)
        {
          digitalWrite(l_led4, HIGH);
        }
        else
        {
          digitalWrite(l_led4, LOW);
        }
       
        lcd.setCursor(12, 5);  
        lcd.print("V4 ");     
        lcd.print(-1* v_in0); 
        
        Serial.print("V4 ");
        Serial.println(v_in0 * -1);
        delay(400);
     }
    }
  }

  if (estadoPulsador0 == HIGH) 
  {
    
    if (v_in0 > v_rms0)
    {
      v_rms0 = v_in0;
      if (v_rms0*0.7071 > 17)
      {
        digitalWrite(l_led4, HIGH);
      }
      else
      {
        digitalWrite(l_led4, LOW);
      }
     
      delay(400);
    }
     lcd.setCursor(12, 5);           
      lcd.print("V4 rms:");               
      lcd.print(v_rms0*0.7071); 

      Serial.print("Vrms4 ");
      Serial.println(v_rms0*0.707);
  }
} 
