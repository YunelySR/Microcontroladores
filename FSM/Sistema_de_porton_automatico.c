//Llamada de las librerías a utilizar
#include <stdio.h>
#include <stdlib.h>


//Macros a utilizar para facilitar el entendimiento del código
#define ESTADO_INIT 0
#define ESTADO_CERRADO 1
#define ESTADO_ABIERTO 2
#define ESTADO_CERRANDO 3
#define ESTADO_ABRIENDO 4
#define ESTADO_ERROR 5
#define TRUE 1
#define FALSE 0
#define RT_MAX 180
#define ERROR_OK 0
#define ERROR_LS 1
#define ERROR_RT 2


//Inicializamos todos los estados temporales en el estado de reseteo
int ESTADO_SIGUIENTE    = ESTADO_INIT;
int ESTADO_ACTUAL       = ESTADO_INIT;
int ESTADO_ANTERIOR     = ESTADO_INIT;


//Estructura de datos para organizar y almacenar las variables utilizadas dentro de la máquina de estados
struct DATA_IO
{
    unsigned int LSC:1;             //Limit switch que indica que el porton está cerrado
    unsigned int LSA:1;             //Limit switch que indica que el porton está abierto
    unsigned int SPP:1;             //Comando pulso-pulso
    unsigned int MA:1;              //Salida que acciona el motor para abrir el porton
    unsigned int MC:1;              //Salida que acciona el motor para cerrar el porton
    unsigned int Cont_RT;           //Contador Run Time en segundos
    unsigned int Led_A:1;           //Led indicador del porton abriendo
    unsigned int Led_C:1;           //Led indicador del porton cerrando
    unsigned int Led_ER:1;          //Led indicador de error
    unsigned int COD_ERR;           //Código de error
    unsigned int DATOS_READY:1;     //Confirmación de recepción de los datos exteriores
}data_io;


//Prototipos de la funciones que se utilizarán en la máquina de estados
int Funcion_INIT(void);
int Funcion_ABIERTO(void);
int Funcion_ABRIENDO(void);
int Funcion_CERRADO(void);
int Funcion_CERRANDO(void);
int Funcion_ERROR(void);


//Función principal
int main()
{
    //Máquina de estado
    for(;;)
    {
        //Estado Init (Estado de reseteo)
        if (ESTADO_SIGUIENTE == ESTADO_INIT)
        {
            ESTADO_SIGUIENTE = Funcion_INIT();
        }

        //Estado abierto
        if (ESTADO_SIGUIENTE == ESTADO_ABIERTO)
        {
            ESTADO_SIGUIENTE = Funcion_ABIERTO();
        }

        //Estado abriendo
        if (ESTADO_SIGUIENTE == ESTADO_ABRIENDO)
        {
            ESTADO_SIGUIENTE = Funcion_ABRIENDO();
        }

        //Estado cerrado
        if (ESTADO_SIGUIENTE == ESTADO_CERRADO)
        {
            ESTADO_SIGUIENTE = Funcion_CERRADO();
        }

        //Estado cerrando
        if (ESTADO_SIGUIENTE == ESTADO_CERRANDO)
        {
            ESTADO_SIGUIENTE = Funcion_CERRANDO();
        }

        //Estado error
        if (ESTADO_SIGUIENTE == ESTADO_ERROR)
        {
            ESTADO_SIGUIENTE = Funcion_ERROR();
        }
        
    }
    return 0;
}


int Funcion_INIT(void)
{
    //Actualización de los estados
    ESTADO_ANTERIOR = ESTADO_INIT;
    ESTADO_ACTUAL = ESTADO_INIT;

    //Actualización de los datos
    data_io.MA = FALSE;
    data_io.MC = FALSE;
    data_io.SPP = FALSE;
    data_io.Led_A = TRUE;
    data_io.Led_C = TRUE;
    data_io.Led_ER = TRUE;
    //delay(100);
    data_io.Led_A = FALSE;
    data_io.Led_C = FALSE;
    data_io.Led_ER = FALSE;
    data_io.COD_ERR = FALSE;
    data_io.Cont_RT = 0;
    data_io.DATOS_READY = FALSE;

    //Confirmación de los datos exteriores
    while (!data_io.DATOS_READY){}

    //Loop infinito
    for(;;)
    {
        //Estado Init ------>> Estado cerrado
        if((data_io.LSC == TRUE) && (data_io.LSA == FALSE))
        {
            return ESTADO_CERRADO;
        }

        //Estado Init ------>> Estado cerrando
        if(((data_io.LSC == FALSE) && (data_io.LSA == FALSE)) || ((data_io.LSC == FALSE) && (data_io.LSA == TRUE)))
        {
            return ESTADO_CERRANDO;
        }

        //Estado Init ------>> Estado error
        if((data_io.LSC == TRUE) && (data_io.LSA == TRUE))
        {
            data_io.COD_ERR = ERROR_LS;
            return ESTADO_ERROR;
        }
    }
}


int Funcion_ABIERTO(void)
{
    //Actualización de los estados
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_ABIERTO;

    //Actualización de los datos
    data_io.MA = FALSE;
    data_io.SPP = FALSE;
    data_io.Led_A = FALSE;
    data_io.Led_C = FALSE;
    data_io.Led_ER = FALSE;

    //Loop infinito
    for(;;)
    {
        //Estado abierto ------>> Estado cerrando
        if (data_io.SPP == TRUE)
        {
            data_io.SPP = FALSE;
            return  ESTADO_CERRANDO;
        }
    }
}


int Funcion_ABRIENDO(void)
{
    //Actualización de los estados
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_ABRIENDO;

    //Actualización de los datos
    data_io.MA = TRUE;
    data_io.Cont_RT = 0;
    data_io.Led_A = TRUE;
    data_io.Led_C = FALSE;
    data_io.Led_ER = FALSE;

    //Loop infinito
    for(;;)
    {
        //Estado abriendo ------>> Estado abierto
        if (data_io.LSA == TRUE)
        {
            return ESTADO_ABIERTO;
        }

        //Estado abriendo ------>> Estado error
        if (data_io.Cont_RT > RT_MAX)
        {
            data_io.COD_ERR = ERROR_RT;
            return ESTADO_ERROR;
        }
    }
}


int Funcion_CERRADO(void)
{
    //Actualización de los estados
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_CERRADO;

    //Actualización de los datos
    data_io.MC = FALSE;
    data_io.SPP = FALSE;
    data_io.Led_A = FALSE;
    data_io.Led_C = FALSE;
    data_io.Led_ER = FALSE;

    //Loop infinito
    for(;;)
    {
        //Estado cerrado ------>> Estado abriendo
        if (data_io.SPP == TRUE)
        {
            data_io.SPP = FALSE;
            return  ESTADO_ABRIENDO;
        }  
    }
}


int Funcion_CERRANDO(void)
{
    //Actualización de los estados
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_CERRANDO;

    //Actualización de los datos
    data_io.MC = TRUE;
    data_io.Cont_RT = 0;
    data_io.Led_A = FALSE;
    data_io.Led_C = TRUE;
    data_io.Led_ER = FALSE;

    //Loop infinito
    for(;;)
    {
        //Estado cerrando ------>> Estado cerrado
        if (data_io.LSC == TRUE)
        {
            return ESTADO_CERRADO;
        }

        //Estado cerrando ------>> Estado error
        if (data_io.Cont_RT > RT_MAX)
        {
            data_io.COD_ERR = ERROR_RT;
            return ESTADO_ERROR;
        } 
    }
}


int Funcion_ERROR(void)
{
    //Actualización de los estados
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_ERROR;

    //Actualización de los datos
    data_io.MC = FALSE;
    data_io.MA = FALSE;
    data_io.SPP = FALSE;
    data_io.Led_A = FALSE;
    data_io.Led_C = FALSE;
    data_io.Led_ER = TRUE;

    //Mensaje indicando al usuario que hubo un error abriendo el porton
    if ((ESTADO_ANTERIOR == ESTADO_ABRIENDO) && (data_io.COD_ERR == ERROR_RT))
    {
        printf("\nERROR ABRIENDO LA PUERTA: REVISE LA POSICION DEL PORTON Y LOS LIMIT SWITCH.\n");
        printf("\nLLUEGO DE HACER LAS REVISIONES PRESIONE EL BOTON PARA RETORNAR AL FUNCIONAMIENTO NORMAL.\n");
    }

    //Mensaje indicando al usuario que hubo un error cerrando el porton
    if ((ESTADO_ANTERIOR == ESTADO_CERRANDO) && (data_io.COD_ERR == ERROR_RT))
    {
        printf("\nERROR CERRANDO LA PUERTA: REVISE LA POSICION DEL PORTON Y LOS LIMIT SWITCH.\n");
        printf("\nLUEGO DE HACER LAS REVISIONES PRESIONE EL BOTON PARA RETORNAR AL FUNCIONAMIENTO NORMAL.\n");
    }

    //Mensaje indicando al usuario que hubo un error inicializando el sistema
    if ((ESTADO_ANTERIOR == ESTADO_INIT) && (data_io.COD_ERR == ERROR_LS))
    {
        printf("\nERROR INICIALIZANDO EL SISTEMA: REVISE LAS CONEXIONES DE LOS SENSORES LIMIT SWITCH.\n");
        printf("\nLUEGO DE ARREGLAR LOS SENSORES PRESIONE EL BOTON PARA REINICIAR EL SISTEMA.\n");
    }
    
    //Loop infinito
    for(;;)
    {
        //Estado error ------>> Estado abriendo
        if ((ESTADO_ANTERIOR == ESTADO_ABRIENDO) && (data_io.SPP == TRUE))
        {
            data_io.SPP = FALSE;
            printf("\nDEVUELTA AL FUNCIONAMIENTO PARA ABRIR LA PUERTA\n");
            data_io.COD_ERR = ERROR_OK;
            return ESTADO_ABRIENDO;
        }

        //Estado error ------>> Estado cerrando
        if ((ESTADO_ANTERIOR == ESTADO_CERRANDO) && (data_io.SPP == TRUE))
        {
            data_io.SPP = FALSE;
            printf("\nDEVUELTA AL FUNCIONAMIENTO PARA CERRAR LA PUERTA\n");
            data_io.COD_ERR = ERROR_OK;
            return ESTADO_CERRANDO;
        }

        //Estado error ------>> Estado Init
        if ((ESTADO_ANTERIOR == ESTADO_INIT) && (data_io.SPP == TRUE))
        {
            data_io.SPP = FALSE;
            printf("\nDEVUELTA AL FUNCIONAMIENTO PARA REINICIAR EL SISTEMA\n");
            data_io.COD_ERR = ERROR_OK;
            return ESTADO_INIT;
        }
    }
}