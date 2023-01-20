/***************************************************
Project Title : Obstacle Avoidance Car
Organisation: Soham Academy Of Human Excellence
Website: https://ria.sohamacademy.org/
***************************************************/

const int trigPin = 12;               //arduino 10th pin connected to trig Pin of the ultrasonic sensor
const int echoPin = 13;               //arduino 11th pin connected to echo Pin of the ultrasonic sensor

const int Input1 = 2;                 //arduino 3rd pin connected to L293D pin input1
const int Input2 = 3;                 //arduino 4th pin connected to L293D pin input2
const int Input3 = 4;                 //arduino 8th pin connected to L293D pin input3
const int Input4 = 5;                 //arduino 9th pin connected to L293D pin input4
const int En1 = 8;
const int En2 = 9;


void setup() 
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode (Input1, OUTPUT);        
  pinMode (Input2, OUTPUT);
  pinMode (Input3, OUTPUT);
  pinMode (Input4, OUTPUT);
  pinMode (En1, OUTPUT);
  pinMode (En2, OUTPUT);
  Serial.begin(9600);
}
long duration, distance;

void loop()
{     
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);  
  duration = pulseIn(echoPin, HIGH);
  distance = duration/58.2;
  Serial.println(distance);
  if(distance>=0 && distance<=10)
   // The speed of sound is 340 m/s or 29 microseconds per centimeter. 
   // The echo travels out and back, so to find the distance of the 
   // object we take half of the distance travelled. 
  {
    digitalWrite(Input1, LOW); 
    digitalWrite(Input2, HIGH); 
    digitalWrite(Input3, HIGH); 
    digitalWrite(Input4, HIGH);
    digitalWrite(En1, HIGH);
    digitalWrite(En2, HIGH);
    delay(100);
    digitalWrite(Input1, LOW); 
    digitalWrite(Input2, HIGH); 
    digitalWrite(Input3, LOW); 
    digitalWrite(Input4, HIGH);
    digitalWrite(En1, HIGH);
    digitalWrite(En2, HIGH);
    delay(200);
  }
  else
  {
    digitalWrite(Input1, HIGH); 
    digitalWrite(Input2, LOW); 
    digitalWrite(Input3, LOW); 
    digitalWrite(Input4, HIGH);
    digitalWrite(En1, HIGH);
    digitalWrite(En2, HIGH);
  }  
  delay(0);
}
