#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "main_asm.h" /* interface to the assembly module */

float lluvia;
float llu;
int boton=0;
int comando=0;
int Estado=0;
int sensor=0;
int sensor2=0;
float lluA=0;
int lluD=0;
int i=0;

//Declarar funciones
void ConfigGPIO(void);
void Abrir(void);
void Cerrar(void);
void Stop(void);
void Revisar(void);
void tiempo(int);
void retardo1m();


void main(void) {
  EnableInterrupts;
  /* include your code here */

  asm_main();		/* call the assembly function */

  for(;;) {
	  lluA=PTAD_PTAD0; //lectura del sensor lluvia analogamente
	  lluD=PTAD_PTAD1; //lectura del sensor lluvia Digital
	  sensor2=PTBD_PTBD0; //Lectura del final de carrera
	  if(comando==1){
		  Revisar();
	  }else if(comando==2){
		  Stop();
	  }
	  if(comando >=3){
		  comando=0;
	  }
	  if(sensor==1){
		  Cerrar();
	  }
	  if(sensor2==1){
		  sensor=0;
	  }
	  
    __RESET_WATCHDOG();	/* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}

void ConfigGPIO(){
	PTBD=0x00;
	PTBDD=0b00001110;
	PTAD=0x00;
	PTADD=0b00001100;
}
void Cerrar(){
	PTBD_PTBD1=1;
	PTBD_PTBD1=0;
}
void Abrir(){
	PTBD_PTBD1=0;
	PTBD_PTBD1=1;
}
void Stop(){
	PTBD_PTBD1=0;
	PTBD_PTBD1=0;
}

void Revisar(){
	Estado=sensor2;//Leer el final de carrera de estado CERRADO
	comando=0;
	if(Estado==1){
		Abrir();
	}else{
		Cerrar();
	}
	tiempo(1000);
}
void tiempo(int milli){
	for(i=0;milli;i++){
		retardo1m();
	}
}

void retardo1m(){
	retardo();
}


interrupt VectorNumber_Vkeyboard void ServicioBoton(){
	tiempo(1000);
	comando++;
}
interrupt VectorNumber_Virq void ServicioSensor(){
	sensor=1;
}
