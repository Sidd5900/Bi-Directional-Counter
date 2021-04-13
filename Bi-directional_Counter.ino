
int count =0;//current count of visitors
int lastcount=0;
float dist,distR,distF;
float time_taken;

// declare variables for the Arduino pins to which the sensor, led and buzzer is connected
int echo1 = 9, trigger1 = 10; //for front ultrasonic sensor
int echo2= 7, trigger2=6;//for rear ultrasonic sensor
int led=2;
int buzzer=3;


void setup()
{
 Serial.begin(9600);
  // initializing the pin to which echo terminal is connected as input 
  and that to which trig terminal is connected as output 
  pinMode(echo1, INPUT);
  pinMode(trigger1, OUTPUT); 
  pinMode(echo2, INPUT);
  pinMode(trigger2, OUTPUT);
  //initializing the pins for the led and buzzer as output
  pinMode(led,OUTPUT);
  pinMode(buzzer,OUTPUT);
}

//function to calculate the distance of an object placed in front of the ultrasonic sensor
void calculate_distance(int trigger, int echo)
{
  digitalWrite(trigger, LOW);
  delayMicroseconds(10);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);

  time_taken = pulseIn(echo, HIGH);
  dist= time_taken*0.034/2;
  if (dist>50)
    dist = 50;
}

void loop()
{
  if(count > 0)   //atleast one visitor present inside
    digitalWrite(led,HIGH);
  else
    digitalWrite(led, LOW);
  if(count > 5)   //count exceeds the threshold value which we have set as 5
    tone(buzzer, 1000); // Send 1KHz sound signal
  else
    noTone(buzzer);
 
  calculate_distance(trigger1,echo1); 
  distF =dist; //get distance of front sensor
 
  if(dist < 50)
  {
    for(int i=0;i<6;i++)
    {
      calculate_distance(trigger2,echo2);
      distR =dist;
      if(dist < 50)
      {
        count++;
        break;  
      }
      delay(200);
    }
  }
  calculate_distance(trigger2,echo2);
  distR =dist; //get distance of rear sensor
  if(distR < 50){
    for(int i=0;i < 5;i++)
    {
      calculate_distance(trigger1,echo1);
      distR =dist;
      if(dist < 50)
      {
        count--;
        break;
      }
      delay(200);
    }
  }
  
  if(count < 0) count cannot be negative
    count = 0;
  
  delay(200);
  //lastcount stores the count of the previous iteration
  if(count>lastcount) //when person enters
  {
     Serial.println("Welcome");
     Serial.print("People in room=");
     Serial.println(count);    
  }
  else if(count<lastcount)  //when person exits
  {
    Serial.println("Thank you for visiting");
    Serial.print("People in room=");
    Serial.println(count);
  }
  lastcount=count; 
}
