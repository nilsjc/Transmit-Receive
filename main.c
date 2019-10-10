/*
 * File:   main.c
 * Author: Nils Finnström-Edvardsson
 */
typedef int bool;
#define true 1
#define false 0
// CONFIG
#pragma config FOSC = INTRCIO   // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select bit (MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <xc.h>
void UART_Init_TR(void);
void UART_Init_RX(void);
void UART_Write(char data);
void UART_Read(void);
void delay(int x);
int convert(void);
int f;
unsigned char c = 1;
bool write = false; // true = RX, false = TR
void main(void) {
    OSCCON = 0b0001;
    TRISA = 0b00000001;
    ANSELbits.ANS0 = 1;     // ANS0 = analog pin
    ADCON0bits.CHS = 0;     // A/D Channel   
    ADCON0bits.ADON = 1;    // Activate A/Domvandlaren
    TRISC = 0x00;
    ADFM = 0;
    
    // Uncomment/comment if needed
    UART_Init_RX();
    
    // Uncomment/comment if needed
    //UART_Init_TR();
    
    while(1){
        
        switch(write){
            case 0:
                UART_Read();
            break;

            case 1:
                UART_Write(c);  // Start transmission
                delay(convert());
                c++;
                PORTC = c;
            break;
        }    
    }
    return;
}
void UART_Init_TR(void){
    TRISB5 = 1;  // As stated in the datasheet
    TRISB7 = 1;  // As stated in the datasheet
    BRGH = 1; // Set For High-Speed Baud Rate
    SPBRG = 25; // Set The Baud Rate To Be 9600 bps
    //--[ Enable The Ascynchronous Serial Port ]--
    SYNC = 0;
    SPEN = 1;
    //--[ Set The RX-TX Pins to be in UART mode (not io) ]--
    TXEN = 1; // Enable UART Transmission
}
void UART_Init_RX()
{
  ANSELHbits.ANS11 = 0;     // ANS0 = analog pin
  BRGH = 1;    // Set For High-Speed Baud Rate
  SPBRG = 25;  // Set The Baud Rate To Be 9600 bps
  // Enable The Ascynchronous Serial Port
  SYNC = 0;
  SPEN = 1;
  // Set The RX-TX Pins to be in UART mode (not io)
  TRISB5 = 1;  // As stated in the datasheet
  TRISB7 = 1;  // As stated in the datasheet
  //--[ Enable UART Receiving Interrupts ]--
  RCIE = 0;    // UART Receving Interrupt Enable Bit
  PEIE = 0;    // Peripherals Interrupt Enable Bit
  GIE = 0;     // Global Interrupt Enable Bit
  //------------------
  CREN = 1;    // Enable Data Continous Reception
 
}
void UART_Write(char data)
{
  while(!TRMT);
  TXREG = data;
}
void UART_Read(void)
{
  while(!RCIF);
  PORTC = RCREG;
}

void delay(int x){
    for(f=0; f<x; f++){
        
    }
}

int convert(void){
    ADCON0bits.GO_DONE = 1;
    while(ADCON0bits.GO_DONE){
        
    }
    return ADRESH;
}




