//header files needed in entire project
#include "tm4c123gh6pm.h"
#include <stdlib.h>
#include <math.h>
#define pi 3.14159265358979323846
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

//functions proto type
void sw1_redled_init(void);//function to initialize port F
unsigned char SW1_value(void);
void redled_on(void);//function to turn the red LED on when distance is>100
void led_on(void);//function to turn the red LED on
void delay_ms(int n);
void delay_us(int n);
void LCD_Cmd(unsigned char command);
void LCD_Data(unsigned char data);
void LCD_String (char *str);
void LCD_int (int n);//function to display integer
void LCD_init(void);
void distance(void);//calculate the distance by knowing the lon&lat of both points
double deg2rad(double deg);
double rad2deg(double rad);
void GPS_init(void);
void readGPSModule(void);
char Uart5_receive(void);
char latitude[10],longitude[10];
int distancee ;
double latt1=0.0,lonn1=0.0,latt2=0.0,lonn2=0.0;




//main functions
int main(void){


sw1_redled_init();
LCD_init();
GPS_init();


        while(1){
            readGPSModule();
            distance();
              LCD_Cmd(0x01);
             LCD_Cmd(0x80);
               delay_ms(100);
             LCD_String("Distance=");
             LCD_int(distancee);
             delay_ms(100);
             redled_on();
                   }


              

}


void sw1_redled_init(void){
SYSCTL_RCGCGPIO_R |= 0x20; /* enable clock to GPIOF */
while((SYSCTL_PRGPIO_R & 0x020) == 0){} ;
GPIO_PORTF_DIR_R = 0x02; /* set PORTF1 pin as output (LED) pin */
GPIO_PORTF_DEN_R = 0x12; /* set PORTF pins 4-1 as digital pins */
GPIO_PORTF_PUR_R = 0x10; /* enable pull up for pin 4 */
GPIO_PORTF_AMSEL_R = 0x00;        // disable analog on PortF
GPIO_PORTF_AFSEL_R = 0x00;        //disable alt funct on Port (F7-0)
GPIO_PORTF_PCTL_R = 0;
}

void redled_on(void){
if(distancee>100){
while(1){
GPIO_PORTF_DATA_R = 0x02;}
}}
//led on whatever distance is
void led_on(void){

while(1){
GPIO_PORTF_DATA_R = 0x02;}
}


void distance(void){
    if ((latt2==0.0) || (lonn2==0.0)) {
          latt2=latt1;
          lonn2=lonn1;
      }
      else {
    double theta1 = deg2rad(latt1-latt2);
    double  theta2 = deg2rad(lonn1-lonn2);
    double a = sin(theta1/2)*sin(theta1/2) + cos(deg2rad(latt2))*cos(deg2rad(latt1))*sin(theta2/2)*sin(theta2/2);
     a = 2*atan2(sqrt(a),sqrt(1-a));

    double temp = (6371 * a)*1000;
    if(temp<=20)
        return;
    distancee+=(int)temp;
    latt2=latt1;
     lonn2=lonn1;}
}



double deg2rad(double deg) {
  return (deg * pi / 180);
}


double rad2deg(double rad) {
  return (rad * 180 / pi);
}

//uart5 init function
void GPS_init(void){
SYSCTL_RCGCUART_R  |= 0x20;  /* enable clock to UART5 */
    SYSCTL_RCGCGPIO_R |= 0x10;  /* enable clock to PORTE for PE4/Rx and RE5/Tx */
    delay_ms(100);
    /* UART5 initialization */
    UART5_CTL_R = 0;         /* UART5 module disbable */
    UART5_IBRD_R = 104;      /* for 9600 baud rate, integer = 104 */
    UART5_FBRD_R = 11;       /* for 9600 baud rate, fractional = 11*/
   // UART5_CC_R = 0;          /select system clock/
    UART5_LCRH_R = 0x70;     /* data lenght 8-bit, not parity bit, enable FIFO */
     UART5_CTL_R |= 0x301;     /* Enable UART5 module, Rx and Tx */

    /* UART5 TX5 and RX5 use PE4 and PE5. Configure them digital and enable alternate function */
    GPIO_PORTE_DEN_R |= 0x30;      /* set PE4 and PE5 as digital */
    GPIO_PORTE_AFSEL_R |= 0x30;    /* Use PE4,PE5 alternate function */
     GPIO_PORTE_AMSEL_R&= ~0x30;    /* Turn off analg function*/
     GPIO_PORTE_PCTL_R= (GPIO_PORTE_PCTL_R&0xFF00FFFF)+0x00110000; /* configure PE4 and PE5 for UART */

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



void delay_ms(int n){
int i,j;
for(i=0;i<n;i++){
for(j=0;j<3180;j++)
{}
}
}
 
void delay_us(int n){
int i,j;
for(i=0;i<n;i++){
for(j=0;j<3;j++)
{}
}
}
unsigned char SW1_value(void){

   return GPIO_PORTF_DATA_R&0x10;

}

char Uart5_receive(void){
while((UART5_FR_R & 0x10) != 0);
return (char)(UART5_DR_R & 0xFF);
}



void readGPSModule(void){
    uint8_t cons;
    uint8_t i = 0;
    double temp;
    char* nothing;
while(1){
    cons = Uart5_receive();

    if(cons == '$')
    {
        cons = Uart5_receive();
        if(cons == 'G')
        {
            cons = Uart5_receive();
            if(cons == 'P')
            {
                cons = Uart5_receive();
                if(cons == 'G')
                {
                    cons = Uart5_receive();
                    if(cons == 'L')
                    {
                        cons= Uart5_receive();
                        if(cons == 'L')
                        {
                            cons= Uart5_receive(); /* For , */
                            cons= Uart5_receive(); /* For 3 */
                            cons= Uart5_receive(); /* For 0 */

                            cons = Uart5_receive();
                            while(cons != ',')
                            {
                                latitude[i] = cons;
                                i++;
                                cons = Uart5_receive();
                            }
                            latitude[i] = '\0';

                           // latitude  calculations
                            temp =strtod(latitude, &nothing);
                            temp /= 60;
                            temp += 30;


                            cons =Uart5_receive(); /* for N */
                            if(cons== 'S')
                                {temp *= -1;}
                                latt1=temp;



                                cons= Uart5_receive(); /* for , */
                                cons= Uart5_receive(); /* for 0 */
                            while(cons == '0')
                            {cons = Uart5_receive();/* for 3 */}
                            cons = Uart5_receive(); /* for 1 */

                            /* longitude calculations*/
                            i=0;
                            cons = Uart5_receive();
                            while(cons!= ',')
                            {
                                longitude[i] = cons;
                                i++;
                                cons= Uart5_receive();
                            }
                            longitude[i] = '\0';


                            temp = strtod(longitude, &nothing);
                            temp /= 60;
                            temp += 31;
                            cons= Uart5_receive(); /* for direction (E or W ) */
                            if(cons== 'W')
                            {temp *= -1;}
                            lonn1=temp;


                            break;}
                        }
                    }
                }
            }
        }

    }
}




