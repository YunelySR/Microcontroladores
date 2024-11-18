namespace Calculadora
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            txtResultado = new TextBox();
            btnUno = new Button();
            btnDos = new Button();
            btnTres = new Button();
            btnSeis = new Button();
            btnCinco = new Button();
            btnCuatro = new Button();
            btnQuitar = new Button();
            btnBorrarTodo = new Button();
            btnBorrar = new Button();
            btnNueve = new Button();
            btnOcho = new Button();
            btnSiete = new Button();
            btnCero = new Button();
            btnElevarCuadrado = new Button();
            btnRaizCuadrada = new Button();
            btnMultiplicar = new Button();
            btnDividir = new Button();
            btnRestar = new Button();
            btnSumar = new Button();
            btnResultado = new Button();
            btnSigno = new Button();
            btnPunto = new Button();
            SuspendLayout();
            // 
            // txtResultado
            // 
            txtResultado.Font = new Font("Segoe UI", 18F, FontStyle.Regular, GraphicsUnit.Point, 0);
            txtResultado.Location = new Point(12, 22);
            txtResultado.MaxLength = 21;
            txtResultado.Name = "txtResultado";
            txtResultado.ReadOnly = true;
            txtResultado.Size = new Size(338, 47);
            txtResultado.TabIndex = 0;
            txtResultado.Text = "0";
            txtResultado.TextAlign = HorizontalAlignment.Right;
            // 
            // btnUno
            // 
            btnUno.Font = new Font("Segoe UI", 16.2F, FontStyle.Regular, GraphicsUnit.Point, 0);
            btnUno.Location = new Point(12, 313);
            btnUno.Name = "btnUno";
            btnUno.Size = new Size(60, 60);
            btnUno.TabIndex = 1;
            btnUno.Text = "1";
            btnUno.UseVisualStyleBackColor = true;
            btnUno.Click += agregarNumero;
            // 
            // btnDos
            // 
            btnDos.Font = new Font("Segoe UI", 16.2F, FontStyle.Regular, GraphicsUnit.Point, 0);
            btnDos.Location = new Point(79, 313);
            btnDos.Name = "btnDos";
            btnDos.Size = new Size(60, 60);
            btnDos.TabIndex = 2;
            btnDos.Text = "2";
            btnDos.UseVisualStyleBackColor = true;
            btnDos.Click += agregarNumero;
            // 
            // btnTres
            // 
            btnTres.Font = new Font("Segoe UI", 16.2F, FontStyle.Regular, GraphicsUnit.Point, 0);
            btnTres.Location = new Point(147, 313);
            btnTres.Name = "btnTres";
            btnTres.Size = new Size(60, 60);
            btnTres.TabIndex = 3;
            btnTres.Text = "3";
            btnTres.UseVisualStyleBackColor = true;
            btnTres.Click += agregarNumero;
            // 
            // btnSeis
            // 
            btnSeis.Font = new Font("Segoe UI", 16.2F, FontStyle.Regular, GraphicsUnit.Point, 0);
            btnSeis.Location = new Point(147, 246);
            btnSeis.Name = "btnSeis";
            btnSeis.Size = new Size(60, 60);
            btnSeis.TabIndex = 6;
            btnSeis.Text = "6";
            btnSeis.UseVisualStyleBackColor = true;
            btnSeis.Click += agregarNumero;
            // 
            // btnCinco
            // 
            btnCinco.Font = new Font("Segoe UI", 16.2F, FontStyle.Regular, GraphicsUnit.Point, 0);
            btnCinco.Location = new Point(79, 246);
            btnCinco.Name = "btnCinco";
            btnCinco.Size = new Size(60, 60);
            btnCinco.TabIndex = 5;
            btnCinco.Text = "5";
            btnCinco.UseVisualStyleBackColor = true;
            btnCinco.Click += agregarNumero;
            // 
            // btnCuatro
            // 
            btnCuatro.Font = new Font("Segoe UI", 16.2F, FontStyle.Regular, GraphicsUnit.Point, 0);
            btnCuatro.Location = new Point(12, 246);
            btnCuatro.Name = "btnCuatro";
            btnCuatro.Size = new Size(60, 60);
            btnCuatro.TabIndex = 4;
            btnCuatro.Text = "4";
            btnCuatro.UseVisualStyleBackColor = true;
            btnCuatro.Click += agregarNumero;
            // 
            // btnQuitar
            // 
            btnQuitar.Font = new Font("Segoe UI", 16.2F, FontStyle.Regular, GraphicsUnit.Point, 0);
            btnQuitar.Location = new Point(147, 111);
            btnQuitar.Name = "btnQuitar";
            btnQuitar.Size = new Size(60, 60);
            btnQuitar.TabIndex = 12;
            btnQuitar.Text = "<-";
            btnQuitar.UseVisualStyleBackColor = true;
            btnQuitar.Click += btnQuitar_Click;
            // 
            // btnBorrarTodo
            // 
            btnBorrarTodo.Font = new Font("Segoe UI", 16.2F, FontStyle.Regular, GraphicsUnit.Point, 0);
            btnBorrarTodo.Location = new Point(79, 111);
            btnBorrarTodo.Name = "btnBorrarTodo";
            btnBorrarTodo.Size = new Size(60, 60);
            btnBorrarTodo.TabIndex = 11;
            btnBorrarTodo.Text = "C";
            btnBorrarTodo.UseVisualStyleBackColor = true;
            btnBorrarTodo.Click += btnBorrarTodo_Click;
            // 
            // btnBorrar
            // 
            btnBorrar.Font = new Font("Segoe UI", 16.2F, FontStyle.Regular, GraphicsUnit.Point, 0);
            btnBorrar.Location = new Point(12, 111);
            btnBorrar.Name = "btnBorrar";
            btnBorrar.Size = new Size(60, 60);
            btnBorrar.TabIndex = 10;
            btnBorrar.Text = "CE";
            btnBorrar.UseVisualStyleBackColor = true;
            btnBorrar.Click += btnBorrar_Click;
            // 
            // btnNueve
            // 
            btnNueve.Font = new Font("Segoe UI", 16.2F, FontStyle.Regular, GraphicsUnit.Point, 0);
            btnNueve.Location = new Point(147, 178);
            btnNueve.Name = "btnNueve";
            btnNueve.Size = new Size(60, 60);
            btnNueve.TabIndex = 9;
            btnNueve.Text = "9";
            btnNueve.UseVisualStyleBackColor = true;
            btnNueve.Click += agregarNumero;
            // 
            // btnOcho
            // 
            btnOcho.Font = new Font("Segoe UI", 16.2F, FontStyle.Regular, GraphicsUnit.Point, 0);
            btnOcho.Location = new Point(79, 178);
            btnOcho.Name = "btnOcho";
            btnOcho.Size = new Size(60, 60);
            btnOcho.TabIndex = 8;
            btnOcho.Text = "8";
            btnOcho.UseVisualStyleBackColor = true;
            btnOcho.Click += agregarNumero;
            // 
            // btnSiete
            // 
            btnSiete.Font = new Font("Segoe UI", 16.2F, FontStyle.Regular, GraphicsUnit.Point, 0);
            btnSiete.Location = new Point(12, 178);
            btnSiete.Name = "btnSiete";
            btnSiete.Size = new Size(60, 60);
            btnSiete.TabIndex = 7;
            btnSiete.Text = "7";
            btnSiete.UseVisualStyleBackColor = true;
            btnSiete.Click += agregarNumero;
            // 
            // btnCero
            // 
            btnCero.Font = new Font("Segoe UI", 16.2F, FontStyle.Regular, GraphicsUnit.Point, 0);
            btnCero.Location = new Point(79, 379);
            btnCero.Name = "btnCero";
            btnCero.Size = new Size(60, 60);
            btnCero.TabIndex = 13;
            btnCero.Text = "0";
            btnCero.UseVisualStyleBackColor = true;
            btnCero.Click += agregarNumero;
            // 
            // btnElevarCuadrado
            // 
            btnElevarCuadrado.Font = new Font("Segoe UI", 16.2F, FontStyle.Regular, GraphicsUnit.Point, 0);
            btnElevarCuadrado.Location = new Point(290, 111);
            btnElevarCuadrado.Name = "btnElevarCuadrado";
            btnElevarCuadrado.Size = new Size(60, 60);
            btnElevarCuadrado.TabIndex = 15;
            btnElevarCuadrado.Tag = "²";
            btnElevarCuadrado.Text = "x²";
            btnElevarCuadrado.UseVisualStyleBackColor = true;
            btnElevarCuadrado.Click += clickOperador;
            // 
            // btnRaizCuadrada
            // 
            btnRaizCuadrada.Font = new Font("Segoe UI", 16.2F, FontStyle.Regular, GraphicsUnit.Point, 0);
            btnRaizCuadrada.Location = new Point(222, 111);
            btnRaizCuadrada.Name = "btnRaizCuadrada";
            btnRaizCuadrada.Size = new Size(60, 60);
            btnRaizCuadrada.TabIndex = 14;
            btnRaizCuadrada.Tag = "√";
            btnRaizCuadrada.Text = "√";
            btnRaizCuadrada.UseVisualStyleBackColor = true;
            btnRaizCuadrada.Click += clickOperador;
            // 
            // btnMultiplicar
            // 
            btnMultiplicar.Font = new Font("Segoe UI", 16.2F, FontStyle.Regular, GraphicsUnit.Point, 0);
            btnMultiplicar.Location = new Point(290, 178);
            btnMultiplicar.Name = "btnMultiplicar";
            btnMultiplicar.Size = new Size(60, 60);
            btnMultiplicar.TabIndex = 17;
            btnMultiplicar.Tag = "X";
            btnMultiplicar.Text = "X";
            btnMultiplicar.UseVisualStyleBackColor = true;
            btnMultiplicar.Click += clickOperador;
            // 
            // btnDividir
            // 
            btnDividir.Font = new Font("Segoe UI", 16.2F, FontStyle.Regular, GraphicsUnit.Point, 0);
            btnDividir.Location = new Point(222, 178);
            btnDividir.Name = "btnDividir";
            btnDividir.Size = new Size(60, 60);
            btnDividir.TabIndex = 16;
            btnDividir.Tag = "÷";
            btnDividir.Text = "÷";
            btnDividir.UseVisualStyleBackColor = true;
            btnDividir.Click += clickOperador;
            // 
            // btnRestar
            // 
            btnRestar.Font = new Font("Segoe UI", 16.2F, FontStyle.Regular, GraphicsUnit.Point, 0);
            btnRestar.Location = new Point(290, 246);
            btnRestar.Name = "btnRestar";
            btnRestar.Size = new Size(60, 60);
            btnRestar.TabIndex = 19;
            btnRestar.Tag = "−";
            btnRestar.Text = "−";
            btnRestar.UseVisualStyleBackColor = true;
            btnRestar.Click += clickOperador;
            // 
            // btnSumar
            // 
            btnSumar.Font = new Font("Segoe UI", 16.2F, FontStyle.Regular, GraphicsUnit.Point, 0);
            btnSumar.Location = new Point(222, 246);
            btnSumar.Name = "btnSumar";
            btnSumar.Size = new Size(60, 60);
            btnSumar.TabIndex = 18;
            btnSumar.Tag = "+";
            btnSumar.Text = "+";
            btnSumar.UseVisualStyleBackColor = true;
            btnSumar.Click += clickOperador;
            // 
            // btnResultado
            // 
            btnResultado.Font = new Font("Segoe UI", 16.2F, FontStyle.Regular, GraphicsUnit.Point, 0);
            btnResultado.Location = new Point(222, 313);
            btnResultado.Name = "btnResultado";
            btnResultado.Size = new Size(129, 125);
            btnResultado.TabIndex = 20;
            btnResultado.Text = "=";
            btnResultado.UseVisualStyleBackColor = true;
            btnResultado.Click += btnResultado_Click;
            // 
            // btnSigno
            // 
            btnSigno.Font = new Font("Segoe UI", 12F, FontStyle.Regular, GraphicsUnit.Point, 0);
            btnSigno.Location = new Point(12, 379);
            btnSigno.Name = "btnSigno";
            btnSigno.Size = new Size(60, 60);
            btnSigno.TabIndex = 21;
            btnSigno.Text = "+/−";
            btnSigno.UseVisualStyleBackColor = true;
            btnSigno.Click += btnSigno_Click;
            // 
            // btnPunto
            // 
            btnPunto.Font = new Font("Segoe UI", 16.2F, FontStyle.Regular, GraphicsUnit.Point, 0);
            btnPunto.Location = new Point(147, 379);
            btnPunto.Name = "btnPunto";
            btnPunto.Size = new Size(60, 60);
            btnPunto.TabIndex = 22;
            btnPunto.Text = ".";
            btnPunto.UseVisualStyleBackColor = true;
            btnPunto.Click += btnPunto_Click;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(364, 450);
            Controls.Add(btnPunto);
            Controls.Add(btnSigno);
            Controls.Add(btnResultado);
            Controls.Add(btnRestar);
            Controls.Add(btnSumar);
            Controls.Add(btnMultiplicar);
            Controls.Add(btnDividir);
            Controls.Add(btnElevarCuadrado);
            Controls.Add(btnRaizCuadrada);
            Controls.Add(btnCero);
            Controls.Add(btnQuitar);
            Controls.Add(btnBorrarTodo);
            Controls.Add(btnBorrar);
            Controls.Add(btnNueve);
            Controls.Add(btnOcho);
            Controls.Add(btnSiete);
            Controls.Add(btnSeis);
            Controls.Add(btnCinco);
            Controls.Add(btnCuatro);
            Controls.Add(btnTres);
            Controls.Add(btnDos);
            Controls.Add(btnUno);
            Controls.Add(txtResultado);
            MaximizeBox = false;
            Name = "Form1";
            StartPosition = FormStartPosition.CenterParent;
            Text = "Calculadora";
            Load += Form1_Load;
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private TextBox txtResultado;
        private Button btnUno;
        private Button btnDos;
        private Button btnTres;
        private Button btnSeis;
        private Button btnCinco;
        private Button btnCuatro;
        private Button btnQuitar;
        private Button btnBorrarTodo;
        private Button btnBorrar;
        private Button btnNueve;
        private Button btnOcho;
        private Button btnSiete;
        private Button btnCero;
        private Button btnElevarCuadrado;
        private Button btnRaizCuadrada;
        private Button btnMultiplicar;
        private Button btnDividir;
        private Button btnRestar;
        private Button btnSumar;
        private Button btnResultado;
        private Button btnSigno;
        private Button btnPunto;
    }
}
