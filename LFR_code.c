#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
void motion_pin_config (void)
{
DDRB = DDRB | 0x0F; //set direction of the PORTB3 to PORTB0 pins as output
PORTB = PORTB & 0xF0; // set initial value of the PORTB3 to PORTB0 pins to logic 0
DDRD = DDRD | 0x30; //Setting PD4 and PD5 pins as output for PWM generation
PORTD = PORTD | 0x30; //PD4 and PD5 pins are for velocity control using PWM
}
void motion_set (unsigned char Direction)
{
unsigned char PortBRestore = 0;
Direction &= 0x0F; // removing upper nibble as it is not needed
PortBRestore = PORTB; // reading the PORTB's original status
PortBRestore &= 0xF0; // setting lower direction nibble to 0
PortBRestore |= Direction; // adding lower nibble for direction command and restoring the PORTB
status
PORTB = PortBRestore; // setting the command to the port
}
void adc_init()
{
ADCSRA = 0x00;
ADMUX = 0x20; //Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
ACSR = 0x80;
ADCSRA = 0x86; //ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0
} //ADC pin configuration void adc_pin_config (void) { DDRA = 0x00; //set PORTF direction as input PORTA = 0x00; //set PORTF pins floating //Function to Initialize PORTS void port_init() { adc_pin_config(); motion_pin_config(); } //TIMER1 initialize - prescale:64 // WGM: 5) PWM 8bit fast, TOP=0x00FF // desired value: 450Hz // actual value: 450.000Hz (0.0%) void timer1_init(void) { TCCR1B = 0x00; //stop TCNT1H = 0xFF; //setup TCNT1L = 0x01; OCR1AH = 0x00; OCR1AL = 0xFF; OCR1BH = 0x00; OCR1BL = 0xFF; ICR1H = 0x00; ICR1L = 0xFF; TCCR1A = 0xA1; TCCR1B = 0x0D; //start Timer
} //This Function accepts the Channel Number and returns the corresponding Analog Value unsigned char ADC_Conversion(unsigned char Ch) { unsigned char a; Ch = Ch & 0x07; ADMUX= 0x20| Ch; ADCSRA = ADCSRA | 0x40; //Set start conversion bit while((ADCSRA&0x10)==0); //Wait for ADC conversion to complete a=ADCH; ADCSRA = ADCSRA|0x10; //clear ADIF (ADC Interrupt Flag) by writing 1 to it return a; } void forward (void) //both wheels forward { motion_set(0x06); } void back (void) //both wheels backward { motion_set(0x09); } void left (void) //Left wheel backward, Right wheel forward { motion_set(0x05); } void right (void) //Left wheel forward, Right wheel backward { motion_set(0x0A); }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ld,rd;
void velocity (int left_motor,int right_motor) // velocity function for controlling the speed of the motors using
PWM signal
{
if(left_motor>0) // speed of the left motor is controlled using input received in the
velocity function
{
OCR1AH = 0x00;
OCR1AL = (unsigned char)left_motor;
ld=0;
}
else if(left_motor < 0) // a flag "ld" is used to save the direction of the motor (i.e; forward or
back)
{
left_motor=left_motor*(-1);
OCR1AH = 0x00;
OCR1AL = (unsigned char)left_motor;
ld=1;
}
if(right_motor>0) // speed of the right motor is controlled using input received in the
velocity function
{
OCR1BH = 0x00;
OCR1BL = (unsigned char)right_motor;
rd=0;
}
else if(right_motor<0) // a flag "rd" is used to save the direction of the motor (i.e; forward or
back)
{
right_motor=right_motor*(-1);
OCR1BH = 0x00;
OCR1BL = (unsigned char)right_motor;
rd=1;
}
if((rd==0)&&(ld==0))
{
forward();
}
else if((rd==1)&&(ld==1))
{
back();
} // the values of "ld" and "rd" are checked to determine the direction to move
else if((rd==0)&&(ld==1))
{
left();
}
else if((rd==1)&&(ld==0))
{
right();
}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void init_devices (void)
{
cli(); //Clears the global interrupts
port_init();
adc_init();
timer1_init();
sei(); //Enables the global interrupts
}
#define thresh 40 //the threshold is set such that if sensor is in the black line its value is > 40
#define lowthresh 25 // the lower threshold is set such that if the sensor is in the white line its value
is < 25
char l,c,r; // variables which store the IR sensor value
int L,C,R; // flag to check the sensor postion (black = 0 ,white = 1 : left, centre or right)
int error; // stores error for proportional part
int PreError; // stores error for derivative part
int Totalerror; // stores error for integral part
int Lr; // variable to control the bot in case of overshoot
float kp = 0.1;
float kd = 1;
float ki = 0.0001;
float PID;
int main()
{
init_devices();
while(1)
{
l = ADC_Conversion(3); //Getting data of Left WL Sensor
c = ADC_Conversion(4); //Getting data of Centre WL Sensor
r = ADC_Conversion(5); //Getting data of Right WL Sensor
//left
if(l>=thresh)
{
L=1;
}
else if(l<lowthresh)
{
L=0;
}
//centre
if(c>=thresh)
{ //the sensor values are compared with the threshold values to assign the flags L,C,R ,which
is further used for motion control
C=1;
}
else if(c<lowthresh)
{
C=0;
}
//right
if(r>=thresh)
{
R=1;
}
else if(r<lowthresh)
{
R=0;
}
if((R==0)&&(L==0)&&(C==0))
{
if(Lr==0)
{
velocity(250,-60); //The bot is moved to the right because of overshoot
}
else if(Lr==1)
{
velocity(-60,250); //The bot is moved to the left because of overshoot
}
}
else
{
if(R==1)
{
Lr=0; // if bot moved to the left due to overshoot, Lr is assigned as zero
}
else if(L==1)
{
Lr=1; // if bot moved to the right due to overshoot, Lr is assigned as zero
}
error = (l - r);
Totalerror += error;
PID = kp*error+ kd*(error-PreError) +ki*Totalerror; // the pid output is calculated
PreError = error;
if(((C==1)&&(R==0)&&(L==0))||((C==1)&&(R==1)&&(L==1)))
{
velocity(255,255); // the bot moves forward if the middle sensor is in the black line,or if
all sensors are in black
}
else
{
velocity(150-PID,150+PID); // for other cases it performs the PID action
}
}
}
}
