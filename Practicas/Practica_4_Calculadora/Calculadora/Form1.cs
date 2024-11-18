/*
 * Yunely Sánchez Ruiz
 * 2021-0282
 * Práctica 4 - Calculadora
*/

namespace Calculadora
{
    public partial class Form1 : Form
    {
        //Variables para almacenar los números y la operación introducidas por el usuario
        double Numero1 = 0, Numero2 = 0;
        char Operador;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }


        //Función para agregar un número al final del número presentado en pantalla
        private void agregarNumero(object sender, EventArgs e)
        {
            //Guardamos el objeto del botón del número que se presionó en esta variable
            var boton = ((Button)sender);


            //Si el texto en pantalla solo está en 0 pues no se agregará otro 0
            if (txtResultado.Text == "0")
            {
                txtResultado.Text = "";
            }

            //Se agregará el número presionado al final de la cola del número presente en pantalla
            txtResultado.Text += boton.Text;
        }


        //Función para guardar el primer número ingresado y la operación que se quiere ejecutar
        private void clickOperador(object sender, EventArgs e)
        {
            //Guardamos el objeto del botón de la operación que se presionó en esta variable
            var boton = ((Button)sender);

            //Convertimos el texto en pantalla en un número y lo guardamos en esta variable
            Numero1 = Convert.ToDouble(txtResultado.Text);

            //Convertimos en carácter el signo de la operación que presionamos
            Operador = Convert.ToChar(boton.Tag);


            //Si el operador fue el de la operación elevar al cuadrado entonces realizará esa operación y convertirá el resultado en texto para poder presentarlo en pantalla
            if (Operador == '²')
            {
                Numero1 = Math.Pow(Numero1, 2);
                txtResultado.Text = Numero1.ToString();
            }

            //Si el operador fue el de la operación raíz cuadrada entonces realizará esa operación y convertirá el resultado en texto para poder presentarlo en pantalla
            else if (Operador == '√')
            {
                //Si el número al que se le quiere calcular la raíz cuadrada no es negativo entonces la operación se realiza sin problemas, de lo contrario no se realiza la operación e imprime un mensaje de advertencia
                if (!txtResultado.Text.Contains("-"))
                {
                    Numero1 = Math.Sqrt(Numero1);
                    txtResultado.Text = Numero1.ToString();
                }

                else
                {
                    MessageBox.Show("No se puede sacar raíz cuadrada a un número negativo.");
                }
            }

            //Si el operador fue el de la operación suma/resta/multiplicación/división entonces limpiará la pantalla para introducir el segundo número de la operación
            else
            {
                txtResultado.Text = "0";
            }
        }


        //Función para calcular el resultado de la operación con los números introducidos previamente
        private void btnResultado_Click(object sender, EventArgs e)
        {
            //Convertimos el texto en pantalla en un número y lo guardamos en esta variable
            Numero2 = Convert.ToDouble(txtResultado.Text);


            //Si el operador presionado previamente fue el de la operación sumar entonces realizará esa operación, convertirá el resultado en texto para poder presentarlo en pantalla y guardará ese resultado como si fuera el primer número introducido para poder seguir utilizándolo en la operación siguiente
            if (Operador == '+')
            {
                txtResultado.Text = (Numero1 + Numero2).ToString();
                Numero1 = Convert.ToDouble(txtResultado.Text);
            }

            //Si el operador presionado previamente fue el de la operación retsar entonces realizará esa operación, convertirá el resultado en texto para poder presentarlo en pantalla y guardará ese resultado como si fuera el primer número introducido para poder seguir utilizándolo en la operación siguiente
            else if (Operador == '−')
            {
                txtResultado.Text = (Numero1 - Numero2).ToString();
                Numero1 = Convert.ToDouble(txtResultado.Text);
            }

            //Si el operador presionado previamente fue el de la operación multiplicar entonces realizará esa operación, convertirá el resultado en texto para poder presentarlo en pantalla y guardará ese resultado como si fuera el primer número introducido para poder seguir utilizándolo en la operación siguiente
            else if (Operador == 'X')
            {
                txtResultado.Text = (Numero1 * Numero2).ToString();
                Numero1 = Convert.ToDouble(txtResultado.Text);
            }

            //Si el operador presionado previamente fue el de la operación dividir entonces realizará esa operación, convertirá el resultado en texto para poder presentarlo en pantalla y guardará ese resultado como si fuera el primer número introducido para poder seguir utilizándolo en la operación siguiente
            else if (Operador == '÷')
            {
                //Si el número divisor no es 0 entonces se realizará la operación, de lo contrario no se realiza la operación e imprime un mensaje de advertencia
                if (txtResultado.Text != "0")
                {
                    txtResultado.Text = (Numero1 / Numero2).ToString();
                    Numero1 = Convert.ToDouble(txtResultado.Text);
                }

                else
                {
                    MessageBox.Show("No se puede dividir por cero!!");
                }
            }
        }


        //Función para eliminar el último número de la cola del número presentado en pantalla
        private void btnQuitar_Click(object sender, EventArgs e)
        {
            //Si el número en pantalla tiene más de un dígito entonces eliminamos el último dígito, sino entonces igualamos el número en pantalla a 0
            if (txtResultado.Text.Length > 1)
            {
                txtResultado.Text = txtResultado.Text.Substring(0, txtResultado.Text.Length - 1);
            }

            else
            {
                txtResultado.Text = "0";
            }
        }


        //Función para borrar los números y operación seleccionados
        private void btnBorrarTodo_Click(object sender, EventArgs e)
        {
            //Igualamos los números almacenados a 0, borramos el operador seleccionado e igualamos a 0 el número en pantalla
            Numero1 = 0;
            Numero2 = 0;
            Operador = '\0';
            txtResultado.Text = "0";
        }


        //Función para borrar el número completo que está en pantalla
        private void btnBorrar_Click(object sender, EventArgs e)
        {
            //Igualamos a 0 el número en pantalla
            txtResultado.Text = "0";
        }


        //Función para agregar el punto decimal al número que escribimos en pantalla
        private void btnPunto_Click(object sender, EventArgs e)
        {
            //Si el número en pantalla no tiene punto decimal entonces se le agrega
            if (!txtResultado.Text.Contains("."))
            {
                txtResultado.Text += ".";
            }
        }


        //Función para cambiar el signo del número que tenemos en pantalla
        private void btnSigno_Click(object sender, EventArgs e)
        {
            //Convertimos el texto en pantalla en un número y lo guardamos en esta variable
            Numero1 = Convert.ToDouble(txtResultado.Text);

            //Multiplicamos por -1 para cambiarle el signo
            Numero1 *= -1;

            //Convertimos el resultado en texto para poder presentarlo en pantalla
            txtResultado.Text = Numero1.ToString();
        }
    }
}
