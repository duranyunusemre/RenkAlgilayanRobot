#include <TimerOne.h>

#define S0     6
#define S1     5
#define S2     4
#define S3     3
#define OUT    2

int   g_count = 0;    // count the frequecy
int   g_array[3];     // store the RGB value
int   g_flag = 0;     // filter of RGB queue
float g_SF[3];        // save the RGB Scale factor
char renk;
// pin tanımlamaları//
#define trigpin 13
#define echopin 7
#define ENBA     8
#define in1      A5
#define in2      9
#define in3      10
#define in4      11
#define ENBB     12
//değişken tanımlaması//
int son,durum;

// Init TSC230 and setting Frequency.
void TSC_Init()
{
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  digitalWrite(S0, LOW);  // OUTPUT FREQUENCY SCALING 2%
  digitalWrite(S1, HIGH); 
}

// Select the filter color 
void TSC_FilterColor(int Level01, int Level02)
{
  if(Level01 != 0)
    Level01 = HIGH;

  if(Level02 != 0)
    Level02 = HIGH;

  digitalWrite(S2, Level01); 
  digitalWrite(S3, Level02); 
}

void TSC_Count()
{
  g_count ++ ;
}

void TSC_Callback()
{
  switch(g_flag)
  {
    case 0: 
         TSC_WB(LOW, LOW);              //Filter without Red
         break;
    case 1:
         g_array[0] = g_count;
         TSC_WB(HIGH, HIGH);            //Filter without Green
         break;
    case 2:
         g_array[1] = g_count;
         TSC_WB(LOW, HIGH);             //Filter without Blue
         break;
    case 3:
         g_array[2] = g_count;
         TSC_WB(HIGH, LOW);             //Clear(no filter)   
         break;
   default:
         g_count = 0;
         break;
  }
}

void TSC_WB(int Level0, int Level1)      //White Balance
{
  g_count = 0;
  g_flag ++;
  TSC_FilterColor(Level0, Level1);
  Timer1.setPeriod(1000);             // set 1s period
}

void setup()
{
  TSC_Init();
  Serial.begin(115200);
  Timer1.initialize();             // defaulte is 1s
  Timer1.attachInterrupt(TSC_Callback);  
  attachInterrupt(0, TSC_Count, RISING);  

  delay(4000);

  for(int i=0; i<3; i++)
    //Serial.println(g_array[i]);

  g_SF[0] = 255.0/ g_array[0];     //R Scale factor
  g_SF[1] = 255.0/ g_array[1] ;    //G Scale factor
  g_SF[2] = 255.0/ g_array[2] ;    //B Scale factor

//  Serial.println(g_SF[0]);
 // Serial.println(g_SF[1]);
 // Serial.println(g_SF[2]);
   pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(ENBA, OUTPUT);
  pinMode(ENBB, OUTPUT);

  digitalWrite(ENBA, HIGH);
    digitalWrite(ENBB, HIGH);
    
}


char RGB()
{

    int R = int(g_array[0] * g_SF[0]);
    int G = int(g_array[1] * g_SF[1]);
    int B = int(g_array[2] * g_SF[2]); 
  int enb = R;
  renk ='R';
 if(G > enb){ renk ='G'; }
 if(B > enb){ renk ='B'; }
 return renk;
}

float sonarSensor ()
{  
  digitalWrite(trigpin, HIGH);
  delayMicroseconds (2);
  digitalWrite(trigpin,LOW);
  delayMicroseconds(10);
  son = pulseIn (echopin, HIGH);
  son = son/58.1;
  return(son);
 }
/////robot dur/////
  void robotdur()
   {
    digitalWrite(ENBA, LOW);
    digitalWrite(ENBB, LOW);
    delay(2000);
    digitalWrite(ENBA, HIGH);
    digitalWrite(ENBB, HIGH);
    }
//////robot ileri fonksiyonu/////  
  void robotileri()
  {
    digitalWrite(in1, HIGH);
      
    digitalWrite(in3, HIGH);
   }
/////robot ileri sağ////
  void robotsagileri()
   {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in4, LOW);
    //analogWrite(in3, 100);
    digitalWrite(in3, HIGH);
    }
/////robot sol ileri///////
void robotsolileri()
{
   analogWrite(in1, 100);
    //digitalWrite(in2, LOW);
    //digitalWrite(in4, LOW);
digitalWrite(in3, LOW);
}


void loop()
{
 g_flag = 0;



durum = sonarSensor ();
  
 if(durum > 0 && durum< 20)
  {
    robotdur();
    
   
    ////////////////
    void TSC_Callback();
   char sonuc = RGB();
   Serial.println(sonuc);
    if (sonuc == 'R')
    {
    robotileri();
     delay(1000);
    }
    ///////////////////
   
    
    robotsagileri();
    delay(1000);
  }
    robotileri();
}
