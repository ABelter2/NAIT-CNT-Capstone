namespace InvKinTester
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
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
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.Btn_Calculate = new System.Windows.Forms.Button();
            this.Num_X = new System.Windows.Forms.NumericUpDown();
            this.Num_Y = new System.Windows.Forms.NumericUpDown();
            this.Num_Z = new System.Windows.Forms.NumericUpDown();
            this.Txt_Theta1 = new System.Windows.Forms.TextBox();
            this.Txt_Theta2 = new System.Windows.Forms.TextBox();
            this.Txt_Theta3 = new System.Windows.Forms.TextBox();
            this.Lbl_Theta1 = new System.Windows.Forms.Label();
            this.Lbl_Theta2 = new System.Windows.Forms.Label();
            this.Lbl_Theta3 = new System.Windows.Forms.Label();
            this.Lbl_Z = new System.Windows.Forms.Label();
            this.Lbl_Y = new System.Windows.Forms.Label();
            this.Lbl_X = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.Num_X)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.Num_Y)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.Num_Z)).BeginInit();
            this.SuspendLayout();
            // 
            // Btn_Calculate
            // 
            this.Btn_Calculate.Location = new System.Drawing.Point(184, 75);
            this.Btn_Calculate.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.Btn_Calculate.Name = "Btn_Calculate";
            this.Btn_Calculate.Size = new System.Drawing.Size(75, 24);
            this.Btn_Calculate.TabIndex = 0;
            this.Btn_Calculate.Text = "Calculate";
            this.Btn_Calculate.UseVisualStyleBackColor = true;
            this.Btn_Calculate.Click += new System.EventHandler(this.Btn_Calculate_Click);
            // 
            // Num_X
            // 
            this.Num_X.Location = new System.Drawing.Point(27, 6);
            this.Num_X.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.Num_X.Name = "Num_X";
            this.Num_X.Size = new System.Drawing.Size(90, 20);
            this.Num_X.TabIndex = 1;
            // 
            // Num_Y
            // 
            this.Num_Y.Location = new System.Drawing.Point(27, 28);
            this.Num_Y.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.Num_Y.Name = "Num_Y";
            this.Num_Y.Size = new System.Drawing.Size(90, 20);
            this.Num_Y.TabIndex = 2;
            // 
            // Num_Z
            // 
            this.Num_Z.Location = new System.Drawing.Point(27, 51);
            this.Num_Z.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.Num_Z.Name = "Num_Z";
            this.Num_Z.Size = new System.Drawing.Size(90, 20);
            this.Num_Z.TabIndex = 3;
            // 
            // Txt_Theta1
            // 
            this.Txt_Theta1.Location = new System.Drawing.Point(184, 6);
            this.Txt_Theta1.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.Txt_Theta1.Name = "Txt_Theta1";
            this.Txt_Theta1.ReadOnly = true;
            this.Txt_Theta1.Size = new System.Drawing.Size(76, 20);
            this.Txt_Theta1.TabIndex = 4;
            // 
            // Txt_Theta2
            // 
            this.Txt_Theta2.Location = new System.Drawing.Point(184, 29);
            this.Txt_Theta2.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.Txt_Theta2.Name = "Txt_Theta2";
            this.Txt_Theta2.ReadOnly = true;
            this.Txt_Theta2.Size = new System.Drawing.Size(76, 20);
            this.Txt_Theta2.TabIndex = 5;
            // 
            // Txt_Theta3
            // 
            this.Txt_Theta3.Location = new System.Drawing.Point(184, 52);
            this.Txt_Theta3.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.Txt_Theta3.Name = "Txt_Theta3";
            this.Txt_Theta3.ReadOnly = true;
            this.Txt_Theta3.Size = new System.Drawing.Size(76, 20);
            this.Txt_Theta3.TabIndex = 6;
            // 
            // Lbl_Theta1
            // 
            this.Lbl_Theta1.AutoSize = true;
            this.Lbl_Theta1.Location = new System.Drawing.Point(133, 9);
            this.Lbl_Theta1.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.Lbl_Theta1.Name = "Lbl_Theta1";
            this.Lbl_Theta1.Size = new System.Drawing.Size(47, 13);
            this.Lbl_Theta1.TabIndex = 7;
            this.Lbl_Theta1.Text = "Theta 1:";
            // 
            // Lbl_Theta2
            // 
            this.Lbl_Theta2.AutoSize = true;
            this.Lbl_Theta2.Location = new System.Drawing.Point(133, 32);
            this.Lbl_Theta2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.Lbl_Theta2.Name = "Lbl_Theta2";
            this.Lbl_Theta2.Size = new System.Drawing.Size(47, 13);
            this.Lbl_Theta2.TabIndex = 8;
            this.Lbl_Theta2.Text = "Theta 2:";
            // 
            // Lbl_Theta3
            // 
            this.Lbl_Theta3.AutoSize = true;
            this.Lbl_Theta3.Location = new System.Drawing.Point(133, 55);
            this.Lbl_Theta3.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.Lbl_Theta3.Name = "Lbl_Theta3";
            this.Lbl_Theta3.Size = new System.Drawing.Size(47, 13);
            this.Lbl_Theta3.TabIndex = 9;
            this.Lbl_Theta3.Text = "Theta 3:";
            // 
            // Lbl_Z
            // 
            this.Lbl_Z.AutoSize = true;
            this.Lbl_Z.Location = new System.Drawing.Point(9, 54);
            this.Lbl_Z.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.Lbl_Z.Name = "Lbl_Z";
            this.Lbl_Z.Size = new System.Drawing.Size(17, 13);
            this.Lbl_Z.TabIndex = 12;
            this.Lbl_Z.Text = "Z:";
            // 
            // Lbl_Y
            // 
            this.Lbl_Y.AutoSize = true;
            this.Lbl_Y.Location = new System.Drawing.Point(9, 29);
            this.Lbl_Y.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.Lbl_Y.Name = "Lbl_Y";
            this.Lbl_Y.Size = new System.Drawing.Size(17, 13);
            this.Lbl_Y.TabIndex = 11;
            this.Lbl_Y.Text = "Y:";
            // 
            // Lbl_X
            // 
            this.Lbl_X.AutoSize = true;
            this.Lbl_X.Location = new System.Drawing.Point(9, 7);
            this.Lbl_X.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.Lbl_X.Name = "Lbl_X";
            this.Lbl_X.Size = new System.Drawing.Size(17, 13);
            this.Lbl_X.TabIndex = 10;
            this.Lbl_X.Text = "X:";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(268, 106);
            this.Controls.Add(this.Lbl_Z);
            this.Controls.Add(this.Lbl_Y);
            this.Controls.Add(this.Lbl_X);
            this.Controls.Add(this.Lbl_Theta3);
            this.Controls.Add(this.Lbl_Theta2);
            this.Controls.Add(this.Lbl_Theta1);
            this.Controls.Add(this.Txt_Theta3);
            this.Controls.Add(this.Txt_Theta2);
            this.Controls.Add(this.Txt_Theta1);
            this.Controls.Add(this.Num_Z);
            this.Controls.Add(this.Num_Y);
            this.Controls.Add(this.Num_X);
            this.Controls.Add(this.Btn_Calculate);
            this.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.Name = "Form1";
            this.Text = "InvKinTest";
            ((System.ComponentModel.ISupportInitialize)(this.Num_X)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.Num_Y)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.Num_Z)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button Btn_Calculate;
        private System.Windows.Forms.NumericUpDown Num_X;
        private System.Windows.Forms.NumericUpDown Num_Y;
        private System.Windows.Forms.NumericUpDown Num_Z;
        private System.Windows.Forms.TextBox Txt_Theta1;
        private System.Windows.Forms.TextBox Txt_Theta2;
        private System.Windows.Forms.TextBox Txt_Theta3;
        private System.Windows.Forms.Label Lbl_Theta1;
        private System.Windows.Forms.Label Lbl_Theta2;
        private System.Windows.Forms.Label Lbl_Theta3;
        private System.Windows.Forms.Label Lbl_Z;
        private System.Windows.Forms.Label Lbl_Y;
        private System.Windows.Forms.Label Lbl_X;
    }
}

