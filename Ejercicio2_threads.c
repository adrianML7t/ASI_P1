#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_TAM 100                 //defino el tamaño maximo a escribir

void get_message(char *buffer);     //funcion que lee un string por pantalla con fgets de forma adecuada
void to_upper(char *str);           //funcion que convierte un string entero a mayusculas
void *pthread1_func();
void *pthread2_func();

int signal_received = 0;            //variable global que se usa como señal entre hilos

int main()
{
    pthread_t th1, th2;

    if (pthread_create(&th1, NULL, pthread1_func, NULL) != 0)
    {
        printf("No se ha podido crear el primer hilo\n");
    }

    if (pthread_create(&th2, NULL, pthread2_func, NULL) != 0)
    {
        printf("No se ha podido crear el segundo hilo\n");
    }

    if (pthread_join(th1, NULL) != 0)
    {
        printf("Error al salir del primer hilo\n");
    }

    if (pthread_join(th2, NULL) != 0)
    {
        printf("Error al salir del segundo hilo\n");
    }
    
    return (0);
}

void *pthread1_func()
{
    char buffer[MAX_TAM];
   
    while(signal_received == 0);
    FILE *fd = fopen("Msg.txt", "r");       
    fread(buffer, sizeof(char), MAX_TAM, fd);
    fclose(fd);

    to_upper(buffer);
    puts(buffer);
    pthread_exit(NULL);
}

void *pthread2_func()
{
    char msg[MAX_TAM];
    get_message(msg);

    FILE *fd;
    fd = fopen("Msg.txt", "w");
    if (fd != NULL)
    {
        fwrite(msg, sizeof(char), sizeof(msg), fd);   
    } 

    else
    {
        printf("No se ha podido crear el fichero\n");
    }
    fclose(fd);
    signal_received = 1;
    pthread_exit(NULL);
}

void get_message(char *buffer)
{
    printf("Introduce la cadena a leer: ");
    fgets(buffer, MAX_TAM, stdin);
    fflush(stdin);

    if (buffer[strlen(buffer) - 1] == '\n')
    {
        buffer[strlen(buffer) - 1] = '\0';
    }
}

void to_upper(char *str)
{
    while (*str != '\0')
    {
        if(*str >= 'a' && *str <= 'z')
        {
            *str = *str - 32;
        }
        str++;
    }
}