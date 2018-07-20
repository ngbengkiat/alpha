// ---------------------------------------------------------------------------------
//   File        :main.c
//   Version     :v1.0.1
//   Date        :06Feb2011
//   Author      :NgBK
//   Description :main()
// ---------------------------------------------------------------------------------

//......................................................................................
// To do list (reminder on what to do in this project)
// test input port pins
// buzzer
// flash data
//......................................................................................

#include "project.h"

int main(void){
	InitIO();	// Must be initialised before any printf()

	clrscr();

   	puts("I'm alive! wehfr");


   	InitDataCold();

	InitDataWarm();

   	//ReadDataFromFlash();

    //StartCMUCAM(4, 57600);
	{
		int16_t pwm = 0;
		char c;
		while(1) {
			printf("\npwm=%4d", pwm);

			SetPWM0(pwm);
			SetPWM1(pwm);
			c = GetChar();
			if (c=='u') {
				pwm += 50;
			}
			else if (c=='d') {
				pwm-= 50;
			}
		}
	}
  //  PrintCamValues();
   // CAM_to_WinM3();
   /// TestMotorMenu();
	MainMenu();

	return 0;
}
