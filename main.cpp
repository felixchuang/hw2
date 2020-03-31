#include "mbed.h"

Serial pc(USBTX, USBRX);
AnalogOut Aout(DAC0_OUT);
AnalogIn Ain(A0);
InterruptIn Switch(SW3);
DigitalOut redLED(LED1);
DigitalOut greenLED(LED2);
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);

char table[11] = {0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F, 0X00};



void Display(int freq);

int main()
{ 
    int i;
    float j;
    int hz = 0;
    int sample = 1000;
    int ADCdata[1000];
    int p[110];
    
    //sampling
    for (i = 0; i < sample; i++){
        ADCdata[i] = Ain;
        wait(1./sample);
    }
    for (i = 0; i < sample; i++){
        pc.printf("%1.3f\r\n", ADCdata[i]);
        wait(0.1);
    }
    // find the peak
    for (i = 0; i < sample; i++){
        if(ADCdata[i]>ADCdata[i+1] && ADCdata[i]>ADCdata[i-1]){
            p[hz]=i;
            hz++;
        }
    }    
    while(1){                
        for (j = 0; j < 2; j += 0.05){
            Aout = 0.5 + 0.5 * sin(j * 3.14159);
            wait(1./(40*hz));
        }
        Display(hz);
        
        if(Switch == 1){
            greenLED = 0;
            redLED = 1;
            display = table[10];
        }
        else{
            greenLED = 1;
            redLED = 0;
            Display(hz);
        }
    }
}

void Display(int freq)
{
    int thou;
    int hund;
    int ten;
    int one;
    char tablel;

    thou = freq / 1000;
    hund = (freq / 100) % 10;
    ten = (freq / 10) % 10;
    one = freq % 10;
    if(thou > 0)
        display = table[thou];
    if(hund > 0)
        display = table[hund];
    wait(0.5);
    display = table[ten];
    wait(0.5);
    tablel = table[one] | 0X80;
    display = tablel;
    wait(0.5);
}

