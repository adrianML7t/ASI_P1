//ASI MONITOR1 ADRIAN MARTINEZ

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/wait.h>

//Funciones de manejo de señales
void SIGUSR1_handler();
void SIGUSR2_handler(int pidmon);
void SIGUSR2_ej4_handler(int pidmon);
void SIGUSR1_ej5_handler();
void SIGUSR2_ej5_handler();
void SIGUSR1_ej6_close();
void SIGALRM_handler();

//Funciones principales 
void *pthread_func1();
void get_clave1(int pidmon);
void get_clave2(int pidmon);
void gestion_procesos(int pidmon);  //claves 3,4,5 y 6
void limpia_buffer();

//Variables globales para comunicacion entre procesos
//Recordar que las variables globales son independientes
//a los distintos procesos
int got_signal = 0;
int num_thread_ej4 = 0;
int num_s2_ej5 = 0;
int fin_prueba = 0;

int main(int argc, char **argv)
{
  int pidmon = atoi(argv[1]);
  get_clave1(pidmon);
  getchar();
  limpia_buffer();
  
  get_clave2(pidmon);
  getchar();
  limpia_buffer();

  gestion_procesos(pidmon); //desbloquea las las claves 3456
  getchar();
  limpia_buffer();
  return (0);
}

void get_clave1(int pidmon)
{
  kill(pidmon, SIGUSR1);
  sleep(5);
  printf("1 terminado, pulsa enter");
}

void get_clave2(int pidmon)
{
  signal(SIGUSR2, SIGUSR2_handler);
  kill(pidmon, SIGUSR1);
  sleep(5);
  kill(pidmon, SIGUSR2);
  printf("2 terminado, pulsa enter");
}

void gestion_procesos(int pidmon)
{
  int pid1, pid2, status1, status2, pstatus;
  kill(pidmon, SIGUSR1);
  sleep(1);
  pid1 = fork();  //Se crea el hijo 1

  if(pid1 != 0)  //Se crea el hijo 2
  {
    pid2 = fork();
  }

  if(pid1 != 0 && pid2 != 0) //Codigo del proceso padre
  {
    kill(pidmon, SIGUSR1);
    printf("Ejercicio 3 terminado");
    signal(SIGUSR1, SIGUSR1_handler); //hijo1 manda s1 al terminar ej4 dando paso al padre
    pause();
    printf("ahora el padre toma el control, pulsa ENTER para tarea 5\n");
    getchar();
    limpia_buffer();
    kill(pidmon, SIGUSR1);

    //cerrar procesos ejercicio6
    printf("pulsa ENTER para cerrar todos los procesos\n");
    getchar();
    limpia_buffer();

    kill(pidmon, SIGUSR1);
    sleep(1);
    kill(pid1, SIGUSR1);    //Los hijos están esperando esta señal para
    kill(pid2, SIGUSR1);    //acabar su ejecucion
    waitpid(pid1, &status1, 0);
    waitpid(pid2, &status2, 0);
    exit(0);
  }

  if (pid1 == 0)  //Codigo de hijo1, para las claves 4, 5 y 6
  {
    signal(SIGUSR2, SIGUSR2_ej4_handler);  //lanza un hilo de ejecucion al recibir s2
    while(got_signal == 0)                 //el bucle se controla con una variable global
    {
      pause();
    }
    printf("ahora el hijo1 ha terminado con el ej4\n");
    kill(getppid(), SIGUSR1);             //avisa al padre de que ha terminado el ejercicio 4

    //el padre manda señal de cierre a hijo1
    signal(SIGUSR1, SIGUSR1_ej6_close);
    pause();
    exit(1);
  }

  //ejercicio5
  if (pid2 == 0)
  {
    signal(SIGUSR1, SIGUSR1_handler);
    pause();
    printf("hijo2 listo para prueba\n");
    //prueba: recibir señales s2 hasta que se reciba s1
    signal(SIGUSR1, SIGUSR1_ej5_handler);
    signal(SIGUSR2, SIGUSR2_ej5_handler);
    while(fin_prueba == 0)
    {
      pause();
      kill(pidmon, SIGUSR2);
    }
    printf("hijo 2 ha terminado con ejercicio 5\n");
  }

  //el padre manda señal de cierre a hijo2
  signal(SIGUSR1, SIGUSR1_ej6_close);
  pause();
  exit(2);
}

void SIGUSR2_handler(int pidmon)
{
}

void SIGUSR1_handler()
{
}

void SIGUSR1_ej6_close()
{
  printf("Cerrando proceso hijo...\n");
}

void SIGUSR2_ej4_handler(int pidmon)   //crea el hilo del ejercicio 4
{
  pthread_t th1;
  pthread_create(&th1, NULL, pthread_func1, NULL);
}

void SIGUSR2_ej5_handler()
{
  num_s2_ej5++;
  printf("Señal de prueba %d recibida\n", num_s2_ej5);
}

void SIGUSR1_ej5_handler()
{
  fin_prueba = 1;
}

void limpia_buffer()
{
  fflush(stdout);
  __fpurge(stdin);
}


/*
Esta funcion aumenta una variable num_thread cada vez que es llamada,
y configura una alarma que mandará la señal SIGALARM. Cuando la funcion
handler de SIGALARM cambie a 1 una variable global, saldrá del bucle y 
el hilo acabará su ejecución
*/
void *pthread_func1()
{
  num_thread_ej4++;
  printf("Hilo numero %d creado\n", num_thread_ej4);
  alarm(20);
  signal(SIGALRM, SIGALRM_handler);
  while(got_signal == 0)
  {
    printf("Hijo 1 activo\n");
    sleep(5);
  }
}

void SIGALRM_handler()
{
  got_signal = 1;
}