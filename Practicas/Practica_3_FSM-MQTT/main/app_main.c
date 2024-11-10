/*
    Practica #3 - FSM-MQTT (Sistema del porton)

    Nombre: Yunely Sánchez Ruiz
    Matrícula: 2021-0282
    Clase: Microcontroladores
    Profesor: Carlos Antonio Pichardo Viuque
*/

/* MQTT (over TCP) Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

//Librerías utilizadas por el ejemplo MQTT
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"

//Librerías llamadas por el usuario
#include "driver/gpio.h"
#include <stdlib.h>
#include "freertos/timers.h"


//Etiquetas creadas para informar al usuario de ciertos eventos a través de la terminal
static const char *TAG = "mqtt_example";    //Etiqueta relacionada a informar sobre los eventos relacionados con el MQTT
static const char *tag = "comunicacion";    //Etiqueta relacionada a informar sobre los eventos con la creación del timer


//Macros a utilizar para facilitar el entendimiento del código
#define ESTADO_INIT 0
#define ESTADO_CERRADO 1
#define ESTADO_ABIERTO 2
#define ESTADO_CERRANDO 3
#define ESTADO_ABRIENDO 4
#define ESTADO_ERROR 5
#define TRUE 1
#define FALSE 0
#define RT_MAX 1200         //2 minutos
#define ERROR_OK 0
#define ERROR_LS 1
#define ERROR_RT 2

//Macros a utilizar para los GPIOs del ESP32
#define SENSOR_ABIERTO 34
#define SENSOR_CERRADO 35
#define MOTOR_ABRIR 32
#define MOTOR_CERRAR 33
#define LED_ABIERTO 25
#define LED_CERRADO 26
#define LED_ERROR 27


//Variables para la interacción recepción/envío con el broker MQTT
esp_mqtt_client_handle_t client;    //Creación del cliente que se conectará con el broker MQTT
int enviar_mensaje;                 //Variable para manejar las funciones de los mensajes para enviar
bool conexion_mqtt = FALSE;         //Variable para verificar la conexión MQTT
bool informar_estado =FALSE;        //Variable para la confirmación del envío del estado actual del porton al celular del usuario


//Inicializamos todos los estados temporales en el estado de reseteo
int ESTADO_SIGUIENTE    = ESTADO_INIT;
int ESTADO_ACTUAL       = ESTADO_INIT;
int ESTADO_ANTERIOR     = ESTADO_INIT;


//Variables que usaremos para crear el timer que se ejecutará cada cierto tiempo
TimerHandle_t xTimers;          //Variable para crear el timer
int periodo_del_timer = 100;    //Periodo de tiempo del timer que se creará
int timerID = 1;                //Identificación única del timer que se usará
bool estado_led_A = FALSE;      //variable creada para el parpadeo del led que indica que el porton se está abriendo
bool estado_led_C = FALSE;      //variable creada para el parpadeo del led que indica que el porton se está cerrando
bool estado_led_E = FALSE;      //variable creada para el parpadeo del led que indica que hay un error con el porton
int contador_led = 0;           //Contador creado para controlar el tiempo del parpadeo de los leds


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


//Función para configurar los GPIOs
void Configuracion_GPIO(void)
{
    gpio_set_direction(SENSOR_ABIERTO, GPIO_MODE_INPUT);
    gpio_set_direction(SENSOR_CERRADO, GPIO_MODE_INPUT);
    gpio_set_direction(MOTOR_ABRIR, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR_CERRAR, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_ABIERTO, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_CERRADO, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_ERROR, GPIO_MODE_OUTPUT);
}


//Función llamada cada cierto tiempo por el timer para actualizar los valores de los GPIOs y las variables de control
void Actualizacion_GPIO(TimerHandle_t pxTimer)
{
    //Encendemos y apagamos todos los leds cada 100 milisegundos durante 3 segundos para probar su funcionamiento durante el inicio del sistema
    if (data_io.Led_A && data_io.Led_C && data_io.Led_ER)
    {
        estado_led_A = !estado_led_A;
        estado_led_C = !estado_led_C;
        estado_led_E = !estado_led_E;
    }
    
    //Parpadeo del led que indica que el porton se está abriendo
    if (data_io.Led_A && (contador_led == 5) && (!data_io.Led_C) && (!data_io.Led_ER))
    {
        estado_led_A = !estado_led_A;
        estado_led_C = FALSE;
        estado_led_E = FALSE; 
    }

    //Parpadeo del led que indica que el porton se está cerrando
    if (data_io.Led_C && (contador_led == 5) && (!data_io.Led_A) && (!data_io.Led_ER))
    {
        estado_led_A = FALSE;
        estado_led_C = !estado_led_C;
        estado_led_E = FALSE; 
    }

    //Parpadeo del led que indica que hay un error con el porton
    if (data_io.Led_ER && (contador_led == 5) && (!data_io.Led_A) && (!data_io.Led_C))
    {
        estado_led_A = FALSE;
        estado_led_C = FALSE;
        estado_led_E = !estado_led_E; 
    }

    //Se apagan todos los leds
    if ((!data_io.Led_A) && (!data_io.Led_C) && (!data_io.Led_ER))
    {
        estado_led_A = FALSE;
        estado_led_C = FALSE;
        estado_led_E = FALSE;
    }
    
    //Calculamos el tiempo para que el parpadeo de los leds sea cada 500 milisegundos
    if (contador_led >= 5)
    {
        contador_led = 0;
    }
    else
    {
        ++contador_led;
    }

    //Actualización de los GPIOs
    data_io.DATOS_READY = FALSE;
    data_io.LSA = gpio_get_level(SENSOR_ABIERTO);
    data_io.LSC = gpio_get_level(SENSOR_CERRADO);
    gpio_set_level(MOTOR_ABRIR, data_io.MA);
    gpio_set_level(MOTOR_CERRAR, data_io.MC);
    gpio_set_level(LED_ABIERTO, estado_led_A);
    gpio_set_level(LED_CERRADO, estado_led_C);
    gpio_set_level(LED_ERROR, estado_led_E);
    data_io.DATOS_READY = TRUE;
    ++data_io.Cont_RT;
}


//Función para crear el timer
esp_err_t Crear_Timer(void)
{
    ESP_LOGI(tag, "\nIniciando configuración del timer.\n");

    //Aquí creamos el timer que utilizaremos para actualizar cada cierto tiempo los GPIOs que estamos usando
    xTimers = xTimerCreate("Timer",                            // Just a text name, not used by the kernel.
                           (pdMS_TO_TICKS(periodo_del_timer)), // The timer period in ticks.
                           pdTRUE,                             // The timers will auto-reload themselves when they expire.
                           (void *)timerID,                    // Assign each timer a unique id equal to its array index.
                           Actualizacion_GPIO                  // Each timer calls the same callback when it expires.
    );

    if (xTimers == NULL)
    {
        // The timer was not created.
        ESP_LOGE(tag, "\nEl timer no fue creado.\n");
    }
    else
    {
        // Start the timer.  No block time is specified, and even if one was
        // it would be ignored because the scheduler has not yet been
        // started.
        if (xTimerStart(xTimers, 0) != pdPASS)
        {
            // The timer could not be set into the Active state.
            ESP_LOGE(tag, "\nNo se pudo configurar el temporizador en el estado Activo.\n");
        }
    }

    return ESP_OK;
}


static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}

/*
 * @brief Event handler registered to receive MQTT events
 *
 *  This function is called by the MQTT client event loop.
 *
 * @param handler_args user data registered to the event.
 * @param base Event base for the handler(always MQTT Base in this example).
 * @param event_id The id for the received event.
 * @param event_data The data for the event, esp_mqtt_event_handle_t.
 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32 "", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");

        msg_id = esp_mqtt_client_subscribe(client, "Sistema del porton/Estado del porton", 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_publish(client, "Estado del porton", " ", 0, 0, 1);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, "Sistema del porton/Boton de control", 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_publish(client, "Sistema del porton/Boton de control", "0", 0, 0, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

        conexion_mqtt = TRUE;   //Confirmamos la conexión MQTT
       

        /*
        msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
        ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
        */
        
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");

        conexion_mqtt = FALSE;   //Confirmamos la desconexión MQTT

        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);

        /*
        msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        */

        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");

        /*
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        */

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        char topico_MQTT [100];                                 //Creamos la variable que nos almcenará el tópico en el que se ha publicado
        strncpy(topico_MQTT, event->topic, event->topic_len);   //Copiamos el nombre del tópico en la variable que creamos anteriormente
        topico_MQTT [event->topic_len] = '\0';                  //Aseguramos de que la cadena de texto copiada esté terminada en '\0'

        char dato_MQTT [100];                                   //Creamos la variable que nos almcenará el dato recibido por MQTT
        strncpy(dato_MQTT, event->data, event->data_len);       //Copiamos el dato recibido por MQTT en la variable que creamos anteriormente
        dato_MQTT [event->data_len] = '\0';                     //Aseguramos de que la cadena de texto copiada esté terminada en '\0'

        //Comparamos el nombre del topico en el que se publicó con el nombre del tópico que nos interesa y el dato recibido con el número 1
        //Si son iguales entonces se ejecuta la sentencia if
        if((strcmp(topico_MQTT, "Sistema del porton/Boton de control")) == (strcmp(dato_MQTT, "1")))
        {
            //Imprimimos en pantalla que se ha mandado a cerrar el porton
            if (ESTADO_ACTUAL == ESTADO_ABIERTO)
            {
                printf("\nMANDATO: CERRAR EL PORTON\n");
            }

            //Imprimimos en pantalla que se ha mandado a abrir el porton
            if (ESTADO_ACTUAL == ESTADO_CERRADO)
            {
                printf("\nMANDATO: ABRIR EL PORTON\n");
            }
            
            //Actualizamos la variable de control para abrir/cerrar el porton
            data_io.SPP = TRUE;
            strcpy(dato_MQTT, "0");
        }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
            ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));

        }
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

static void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = CONFIG_BROKER_URL,
    };
#if CONFIG_BROKER_URL_FROM_STDIN
    char line[128];

    if (strcmp(mqtt_cfg.broker.address.uri, "FROM_STDIN") == 0) {
        int count = 0;
        printf("Please enter url of mqtt broker\n");
        while (count < 128) {
            int c = fgetc(stdin);
            if (c == '\n') {
                line[count] = '\0';
                break;
            } else if (c > 0 && c < 127) {
                line[count] = c;
                ++count;
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
        mqtt_cfg.broker.address.uri = line;
        printf("Broker url: %s\n", line);
    } else {
        ESP_LOGE(TAG, "Configuration mismatch: wrong broker url");
        abort();
    }
#endif /* CONFIG_BROKER_URL_FROM_STDIN */

    client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

void app_main(void)
{
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("mqtt_client", ESP_LOG_VERBOSE);
    esp_log_level_set("mqtt_example", ESP_LOG_VERBOSE);
    esp_log_level_set("transport_base", ESP_LOG_VERBOSE);
    esp_log_level_set("esp-tls", ESP_LOG_VERBOSE);
    esp_log_level_set("transport", ESP_LOG_VERBOSE);
    esp_log_level_set("outbox", ESP_LOG_VERBOSE);

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());


    //Llamamos a esta función para conectarnos al broker MQTT
    mqtt_app_start();

    //Llamamos a la función para configurar los GPIOs
    Configuracion_GPIO();

    //LLamamos a la función que creará el timer
    Crear_Timer();


    //Máquina de estado
    for(;;)
    {
        //Estado Init (Estado de inicio/reseteo)
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
}


int Funcion_INIT(void)
{
    //Actualización de los estados
    ESTADO_ANTERIOR = ESTADO_INIT;
    ESTADO_ACTUAL = ESTADO_INIT;
    printf("\nESTADO ACTUAL: ESTADO INIT\n");
    informar_estado = FALSE;

    //Actualización de los datos
    data_io.MA = FALSE;
    data_io.MC = FALSE;
    data_io.SPP = FALSE;
    data_io.COD_ERR = FALSE;
    data_io.Cont_RT = 0;

    //Prueba de funcionamiento de los leds
    data_io.Led_A = TRUE;
    data_io.Led_C = TRUE;
    data_io.Led_ER = TRUE;
    vTaskDelay(3000/portTICK_PERIOD_MS);
    data_io.Led_A = FALSE;
    data_io.Led_C = FALSE;
    data_io.Led_ER = FALSE;

    //Confirmación de los datos exteriores
    while (!data_io.DATOS_READY){}


    //Loop infinito
    for(;;)
    {
        vTaskDelay(10/portTICK_PERIOD_MS);

        //Confirmamos la conexión MQTT antes de enviar el mensaje al usuario
        if ((conexion_mqtt == TRUE) && (informar_estado == FALSE))
        {
            //Enviamos el estado actual en el que se encuentra el porton al celular por MQTT
            enviar_mensaje = esp_mqtt_client_publish(client, "Sistema del porton/Estado del porton", "INICIANDO SISTEMA...", 0, 0, 1);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", enviar_mensaje);
            informar_estado = TRUE;
        }


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
    printf("\nESTADO ACTUAL: ESTADO ABIERTO\n");
    informar_estado = FALSE;

    //Actualización de los datos
    data_io.MA = FALSE;
    data_io.SPP = FALSE;
    data_io.Led_A = FALSE;
    data_io.Led_C = FALSE;
    data_io.Led_ER = FALSE;


    //Loop infinito
    for(;;)
    {
        vTaskDelay(10/portTICK_PERIOD_MS);

        //Confirmamos la conexión MQTT antes de enviar el mensaje al usuario
        if ((conexion_mqtt == TRUE) && (informar_estado == FALSE))
        {
            //Enviamos el estado actual en el que se encuentra el porton al celular por MQTT
            enviar_mensaje = esp_mqtt_client_publish(client, "Sistema del porton/Estado del porton", "ABIERTO", 0, 0, 1);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", enviar_mensaje);
            informar_estado = TRUE;
        }


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
    printf("\nESTADO ACTUAL: ESTADO ABRIENDO\n");
    informar_estado = FALSE;

    //Actualización de los datos
    data_io.MA = TRUE;
    data_io.Cont_RT = 0;
    data_io.Led_C = FALSE;
    data_io.Led_ER = FALSE;
    data_io.Led_A = TRUE;

    //Tiempo de separación del porton de los limit switch
    vTaskDelay(3000/portTICK_PERIOD_MS);


    //Loop infinito
    for(;;)
    {
        vTaskDelay(10/portTICK_PERIOD_MS);

        //Confirmamos la conexión MQTT antes de enviar el mensaje al usuario
        if ((conexion_mqtt == TRUE) && (informar_estado == FALSE))
        {
            //Enviamos el estado actual en el que se encuentra el porton al celular por MQTT
            enviar_mensaje = esp_mqtt_client_publish(client, "Sistema del porton/Estado del porton", "ABRIENDO", 0, 0, 1);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", enviar_mensaje);
            informar_estado = TRUE;
        }


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
    printf("\nESTADO ACTUAL: ESTADO CERRADO\n");
    informar_estado = FALSE;

    //Actualización de los datos
    data_io.MC = FALSE;
    data_io.SPP = FALSE;
    data_io.Led_A = FALSE;
    data_io.Led_C = FALSE;
    data_io.Led_ER = FALSE;


    //Loop infinito
    for(;;)
    {
        vTaskDelay(10/portTICK_PERIOD_MS);

        //Confirmamos la conexión MQTT antes de enviar el mensaje al usuario
        if ((conexion_mqtt == TRUE) && (informar_estado == FALSE))
        {
            //Enviamos el estado actual en el que se encuentra el porton al celular por MQTT
            enviar_mensaje = esp_mqtt_client_publish(client, "Sistema del porton/Estado del porton", "CERRADO", 0, 0, 1);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", enviar_mensaje);
            informar_estado = TRUE;
        }


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
    printf("\nESTADO ACTUAL: ESTADO CERRANDO\n");
    informar_estado = FALSE;

    //Actualización de los datos
    data_io.MC = TRUE;
    data_io.Cont_RT = 0;
    data_io.Led_A = FALSE;
    data_io.Led_ER = FALSE;
    data_io.Led_C = TRUE;

    //Tiempo de separación del porton de los limit switch
    vTaskDelay(3000/portTICK_PERIOD_MS);


    //Loop infinito
    for(;;)
    {
        vTaskDelay(10/portTICK_PERIOD_MS);

        //Confirmamos la conexión MQTT antes de enviar el mensaje al usuario
        if ((conexion_mqtt == TRUE) && (informar_estado == FALSE))
        {
            //Enviamos el estado actual en el que se encuentra el porton al celular por MQTT
            enviar_mensaje = esp_mqtt_client_publish(client, "Sistema del porton/Estado del porton", "CERRANDO", 0, 0, 1);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", enviar_mensaje);
            informar_estado = TRUE;
        }


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
    printf("\nESTADO ACTUAL: ESTADO ERROR\n");
    informar_estado = FALSE;

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
        vTaskDelay(10/portTICK_PERIOD_MS);
        
        //Confirmamos la conexión MQTT antes de enviar el mensaje al usuario
        if ((conexion_mqtt == TRUE) && (informar_estado == FALSE))
        {
            //Enviamos el estado actual en el que se encuentra el porton al celular por MQTT
            enviar_mensaje = esp_mqtt_client_publish(client, "Sistema del porton/Estado del porton", "¡HAY UN ERROR! REVISE LA POSICIÓN DEL PORTON Y LOS LIMIT SWITCHES.", 0, 0, 1);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", enviar_mensaje);
            informar_estado = TRUE;
        }


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