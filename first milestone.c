//header files needed in entire project
#include "tm4c123gh6pm.h"
#include <stdlib.h>
#include <math.h>
#define pi 3.14159265358979323846
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

//functions proto type
void sw1_greenled_init(void);//function to initialize port F
unsigned char SW1_value(void);
void greenled_on(void);//function to turn the green LED on when distance is>100
void delay_ms(int n);
void delay_us(int n);
void LCD_Cmd(unsigned char command);
void LCD_Data(unsigned char data);
void LCD_String (char *str);
void LCD_int (int n);
void LCD_init(void);
double distance(double lat1, double lon1, double lat2, double lon2, char unit);//calculate the distance by knowing the lon&lat of both points
double deg2rad(double deg);
double rad2deg(double rad);
double distancee ;




//main functions
int main(void){

sw1_greenled_init();
LCD_init();

		while(1){
			
				LCD_Cmd(0x01); 
				LCD_Cmd(0x80); 
				delay_ms(500);
				LCD_Data('t');
				LCD_Data('o');
				LCD_Data('t');
				LCD_Data('a');
				LCD_Data('l');
			        
				delay_ms(500);
			
			
		}


}


void sw1_greenled_init(void){
SYSCTL_RCGCGPIO_R |= 0x20; /* enable clock to GPIOF */
while((SYSCTL_PRGPIO_R & 0x020) == 0){} ;
GPIO_PORTF_DIR_R = 0x08; /* set PORTF3 pin as output (LED) pin */
GPIO_PORTF_DEN_R = 0x18; /* set PORTF pins 4-3 as digital pins */
GPIO_PORTF_PUR_R = 0x10; /* enable pull up for pin 4 */
GPIO_PORTF_AMSEL_R = 0x00;        // disable analog on PortF
GPIO_PORTF_AFSEL_R = 0x00;        //disable alt funct on Port (F7-0)
GPIO_PORTF_PCTL_R = 0;
}

void greenled_on(void){
if(distancee>100){
while(1){
GPIO_PORTF_DATA_R = 0x08;}
}}

double distance(double lat1, double lon1, double lat2, double lon2, char unit) {
  double theta, dist;
  if ((lat1 == lat2) && (lon1 == lon2)) {
    return 0;
  }
  else {
    theta = lon1 - lon2;
    dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
    dist = acos(dist);
    dist = rad2deg(dist);
    dist = dist * 60 * 1.1515;
    switch(unit) {
      case 'M':
        break;
      case 'm'://to get distance in meter
        dist = dist * 1.609344*1000;
        break;
      case 'N':
        dist = dist * 0.8684;
        break;
    }
    return (dist);
  }
}



double deg2rad(double deg) {
  return (deg * pi / 180);
}


double rad2deg(double rad) {
  return (rad * 180 / pi);
}

void LCD_init(void){

		SYSCTL_RCGCGPIO_R |= 0x03; // Enable clock for PORTB and PORTA
    while((SYSCTL_PRGPIO_R & 0x03) == 0){} ;

    //GPIO_PORTB_LOCK_R = 0x4C4F434B;
    //GPIO_PORTA_LOCK_R = 0x4C4F434B;
    GPIO_PORTB_CR_R |= 0xFF;            // Unlocking PORTB PIN 1...8;
    GPIO_PORTA_CR_R |= 0xE0;            // Unlocking PORTA PIN 5 6 7;

    GPIO_PORTB_DEN_R  |= 0xFF;
    GPIO_PORTB_AMSEL_R &= ~0xFF;
    GPIO_PORTA_DEN_R  |= 0xE0;
    GPIO_PORTA_AMSEL_R &= ~0xE0;

    GPIO_PORTB_DIR_R  |= 0xFF;   // Configure PORTB Pin 1~8 digital output pins
    GPIO_PORTA_DIR_R  |= 0xE0;  // Configure PORTA Pin 5, 6 and 7 digital input pins


    GPIO_PORTB_AFSEL_R  = 0x00;
    GPIO_PORTB_PCTL_R   = 0x00000000;
    GPIO_PORTA_AFSEL_R &= ~0xE0;
    GPIO_PORTA_PCTL_R  &= ~0xFFF00000;
			
		delay_ms(20); /* initialization sequence */
		LCD_Cmd(0x30);
		delay_ms(5);
		LCD_Cmd(0x30);
		delay_us(100);
		LCD_Cmd(0x30);

    LCD_Cmd(0x38); //8-bits,2 display lines, 5x7 font

   LCD_Cmd(0x06); //increments automatically

    LCD_Cmd(0x0F); //Turn on display

   LCD_Cmd(0x01); //clear display
}

void LCD_Data(unsigned char data){

    GPIO_PORTA_DATA_R |= 0x20;  // RS => 1  TO SELECT Data
    GPIO_PORTB_DATA_R  = data;  // put command in the data
    GPIO_PORTA_DATA_R |= 0x80;  // ENABLE IS SET TO 1
    delay_us(0);
    GPIO_PORTA_DATA_R &= ~0xE0; // ENABLE IS SET TO 0
	delay_us(40);
} 
void LCD_String (char *str)	// Send string to LCD function 
{
	int i;
	for(i=0;str[i]!=0;i++)   //Send each char of string till the NULL 
	{
		LCD_Data(str[i]);   //Call LCD data write 
	}
}
void LCD_int (int n){
 int h,t,u;  // hundreds,tens,units

    u=n%10;
    t=(n/10)%10;
    h=(n/100)%10;
    LCD_Data(h+'0');
    LCD_Data(t+'0');
   LCD_Data(u+'0');


}

void LCD_Cmd(unsigned char command){

    GPIO_PORTA_DATA_R &= ~0xE0; // RS,R/W => 0  TO SELECT COMMAND AND WRITE
    GPIO_PORTB_DATA_R  = command;  // put command in the data
    GPIO_PORTA_DATA_R |= 0x80;  // Enable is set
    delay_us(0);
    GPIO_PORTA_DATA_R &= ~0xE0;
	if (command < 4){
	delay_ms(2); // command 1 and 2 needs up to 1.64ms //
	}		
	else{
	delay_us(40); /* all others 40 us */
} 
}


int i,j;
void delay_ms(int n){

for(i=0;i<n;i++){
for(j=0;j<3180;j++)
{}
}
}
 
void delay_us(int n){

for(i=0;i<n;i++){
for(j=0;j<3;j++)
{}
}
}
unsigned char SW1_value(void){

   return GPIO_PORTF_DATA_R&0x10;

}



