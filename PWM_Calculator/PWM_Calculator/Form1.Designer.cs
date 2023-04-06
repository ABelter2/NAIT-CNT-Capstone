
namespace PWM_Calculator
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
            this.UI_Freq_TxtBx = new System.Windows.Forms.TextBox();
            this.UI_Freq_Lbl = new System.Windows.Forms.Label();
            this.UI_Per_Lbl = new System.Windows.Forms.Label();
            this.UI_Per_TxtBx = new System.Windows.Forms.TextBox();
            this.UI_Bus_Lbl = new System.Windows.Forms.Label();
            this.UI_Bus_TxtBx = new System.Windows.Forms.TextBox();
            this.UI_FindFactors_Btn = new System.Windows.Forms.Button();
            this.UI_Stage1_Lbl = new System.Windows.Forms.Label();
            this.UI_Stage1_TxtBx = new System.Windows.Forms.TextBox();
            this.UI_Stage2_Lbl = new System.Windows.Forms.Label();
            this.UI_Stage2_TxtBx = new System.Windows.Forms.TextBox();
            this.UI_CalcPer_CheckBx = new System.Windows.Forms.CheckBox();
            this.UI_ResultFreq_Lbl = new System.Windows.Forms.Label();
            this.UI_ResultFreq_TxtBx = new System.Windows.Forms.TextBox();
            this.UI_20MHzBusRate_Btn = new System.Windows.Forms.Button();
            this.UI_16BitMode_CheckBx = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // UI_Freq_TxtBx
            // 
            this.UI_Freq_TxtBx.Location = new System.Drawing.Point(15, 90);
            this.UI_Freq_TxtBx.Name = "UI_Freq_TxtBx";
            this.UI_Freq_TxtBx.Size = new System.Drawing.Size(105, 20);
            this.UI_Freq_TxtBx.TabIndex = 1;
            // 
            // UI_Freq_Lbl
            // 
            this.UI_Freq_Lbl.AutoSize = true;
            this.UI_Freq_Lbl.Location = new System.Drawing.Point(12, 74);
            this.UI_Freq_Lbl.Name = "UI_Freq_Lbl";
            this.UI_Freq_Lbl.Size = new System.Drawing.Size(108, 13);
            this.UI_Freq_Lbl.TabIndex = 7;
            this.UI_Freq_Lbl.Text = "Pulse Frequency (Hz)";
            // 
            // UI_Per_Lbl
            // 
            this.UI_Per_Lbl.AutoSize = true;
            this.UI_Per_Lbl.Location = new System.Drawing.Point(12, 113);
            this.UI_Per_Lbl.Name = "UI_Per_Lbl";
            this.UI_Per_Lbl.Size = new System.Drawing.Size(98, 13);
            this.UI_Per_Lbl.TabIndex = 8;
            this.UI_Per_Lbl.Text = "Period Value (ticks)";
            // 
            // UI_Per_TxtBx
            // 
            this.UI_Per_TxtBx.Location = new System.Drawing.Point(15, 129);
            this.UI_Per_TxtBx.Name = "UI_Per_TxtBx";
            this.UI_Per_TxtBx.Size = new System.Drawing.Size(105, 20);
            this.UI_Per_TxtBx.TabIndex = 2;
            // 
            // UI_Bus_Lbl
            // 
            this.UI_Bus_Lbl.AutoSize = true;
            this.UI_Bus_Lbl.Location = new System.Drawing.Point(12, 9);
            this.UI_Bus_Lbl.Name = "UI_Bus_Lbl";
            this.UI_Bus_Lbl.Size = new System.Drawing.Size(73, 13);
            this.UI_Bus_Lbl.TabIndex = 6;
            this.UI_Bus_Lbl.Text = "Bus Rate (Hz)";
            // 
            // UI_Bus_TxtBx
            // 
            this.UI_Bus_TxtBx.Location = new System.Drawing.Point(15, 25);
            this.UI_Bus_TxtBx.Name = "UI_Bus_TxtBx";
            this.UI_Bus_TxtBx.Size = new System.Drawing.Size(105, 20);
            this.UI_Bus_TxtBx.TabIndex = 0;
            // 
            // UI_FindFactors_Btn
            // 
            this.UI_FindFactors_Btn.Location = new System.Drawing.Point(164, 129);
            this.UI_FindFactors_Btn.Name = "UI_FindFactors_Btn";
            this.UI_FindFactors_Btn.Size = new System.Drawing.Size(105, 23);
            this.UI_FindFactors_Btn.TabIndex = 3;
            this.UI_FindFactors_Btn.Text = "Find Factors";
            this.UI_FindFactors_Btn.UseVisualStyleBackColor = true;
            this.UI_FindFactors_Btn.Click += new System.EventHandler(this.UI_FindFactors_Btn_Click);
            // 
            // UI_Stage1_Lbl
            // 
            this.UI_Stage1_Lbl.AutoSize = true;
            this.UI_Stage1_Lbl.Location = new System.Drawing.Point(161, 9);
            this.UI_Stage1_Lbl.Name = "UI_Stage1_Lbl";
            this.UI_Stage1_Lbl.Size = new System.Drawing.Size(82, 13);
            this.UI_Stage1_Lbl.TabIndex = 9;
            this.UI_Stage1_Lbl.Text = "PrescaleStage1";
            // 
            // UI_Stage1_TxtBx
            // 
            this.UI_Stage1_TxtBx.Location = new System.Drawing.Point(164, 25);
            this.UI_Stage1_TxtBx.Name = "UI_Stage1_TxtBx";
            this.UI_Stage1_TxtBx.ReadOnly = true;
            this.UI_Stage1_TxtBx.Size = new System.Drawing.Size(105, 20);
            this.UI_Stage1_TxtBx.TabIndex = 4;
            // 
            // UI_Stage2_Lbl
            // 
            this.UI_Stage2_Lbl.AutoSize = true;
            this.UI_Stage2_Lbl.Location = new System.Drawing.Point(161, 48);
            this.UI_Stage2_Lbl.Name = "UI_Stage2_Lbl";
            this.UI_Stage2_Lbl.Size = new System.Drawing.Size(82, 13);
            this.UI_Stage2_Lbl.TabIndex = 10;
            this.UI_Stage2_Lbl.Text = "PrescaleStage2";
            // 
            // UI_Stage2_TxtBx
            // 
            this.UI_Stage2_TxtBx.Location = new System.Drawing.Point(164, 64);
            this.UI_Stage2_TxtBx.Name = "UI_Stage2_TxtBx";
            this.UI_Stage2_TxtBx.ReadOnly = true;
            this.UI_Stage2_TxtBx.Size = new System.Drawing.Size(105, 20);
            this.UI_Stage2_TxtBx.TabIndex = 5;
            // 
            // UI_CalcPer_CheckBx
            // 
            this.UI_CalcPer_CheckBx.AutoSize = true;
            this.UI_CalcPer_CheckBx.Location = new System.Drawing.Point(15, 158);
            this.UI_CalcPer_CheckBx.Name = "UI_CalcPer_CheckBx";
            this.UI_CalcPer_CheckBx.Size = new System.Drawing.Size(103, 17);
            this.UI_CalcPer_CheckBx.TabIndex = 11;
            this.UI_CalcPer_CheckBx.Text = "Calculate Period";
            this.UI_CalcPer_CheckBx.UseVisualStyleBackColor = true;
            this.UI_CalcPer_CheckBx.CheckedChanged += new System.EventHandler(this.UI_CalcPer_CheckBx_CheckedChanged);
            // 
            // UI_ResultFreq_Lbl
            // 
            this.UI_ResultFreq_Lbl.AutoSize = true;
            this.UI_ResultFreq_Lbl.Location = new System.Drawing.Point(161, 87);
            this.UI_ResultFreq_Lbl.Name = "UI_ResultFreq_Lbl";
            this.UI_ResultFreq_Lbl.Size = new System.Drawing.Size(104, 13);
            this.UI_ResultFreq_Lbl.TabIndex = 13;
            this.UI_ResultFreq_Lbl.Text = "Resulting Frequency";
            // 
            // UI_ResultFreq_TxtBx
            // 
            this.UI_ResultFreq_TxtBx.Location = new System.Drawing.Point(164, 103);
            this.UI_ResultFreq_TxtBx.Name = "UI_ResultFreq_TxtBx";
            this.UI_ResultFreq_TxtBx.ReadOnly = true;
            this.UI_ResultFreq_TxtBx.Size = new System.Drawing.Size(105, 20);
            this.UI_ResultFreq_TxtBx.TabIndex = 12;
            // 
            // UI_20MHzBusRate_Btn
            // 
            this.UI_20MHzBusRate_Btn.Location = new System.Drawing.Point(15, 48);
            this.UI_20MHzBusRate_Btn.Name = "UI_20MHzBusRate_Btn";
            this.UI_20MHzBusRate_Btn.Size = new System.Drawing.Size(105, 23);
            this.UI_20MHzBusRate_Btn.TabIndex = 14;
            this.UI_20MHzBusRate_Btn.Text = "BusRate = 20MHz";
            this.UI_20MHzBusRate_Btn.UseVisualStyleBackColor = true;
            this.UI_20MHzBusRate_Btn.Click += new System.EventHandler(this.UI_20MHzBusRate_Btn_Click);
            // 
            // UI_16BitMode_CheckBx
            // 
            this.UI_16BitMode_CheckBx.AutoSize = true;
            this.UI_16BitMode_CheckBx.Location = new System.Drawing.Point(162, 158);
            this.UI_16BitMode_CheckBx.Name = "UI_16BitMode_CheckBx";
            this.UI_16BitMode_CheckBx.Size = new System.Drawing.Size(83, 17);
            this.UI_16BitMode_CheckBx.TabIndex = 15;
            this.UI_16BitMode_CheckBx.Text = "16-Bit Mode";
            this.UI_16BitMode_CheckBx.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(278, 185);
            this.Controls.Add(this.UI_16BitMode_CheckBx);
            this.Controls.Add(this.UI_20MHzBusRate_Btn);
            this.Controls.Add(this.UI_ResultFreq_Lbl);
            this.Controls.Add(this.UI_ResultFreq_TxtBx);
            this.Controls.Add(this.UI_CalcPer_CheckBx);
            this.Controls.Add(this.UI_Stage1_Lbl);
            this.Controls.Add(this.UI_Stage1_TxtBx);
            this.Controls.Add(this.UI_Stage2_Lbl);
            this.Controls.Add(this.UI_Stage2_TxtBx);
            this.Controls.Add(this.UI_FindFactors_Btn);
            this.Controls.Add(this.UI_Bus_Lbl);
            this.Controls.Add(this.UI_Bus_TxtBx);
            this.Controls.Add(this.UI_Per_Lbl);
            this.Controls.Add(this.UI_Per_TxtBx);
            this.Controls.Add(this.UI_Freq_Lbl);
            this.Controls.Add(this.UI_Freq_TxtBx);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Form1";
            this.Text = "PWM Prescale Calculator - ABelter";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox UI_Freq_TxtBx;
        private System.Windows.Forms.Label UI_Freq_Lbl;
        private System.Windows.Forms.Label UI_Per_Lbl;
        private System.Windows.Forms.TextBox UI_Per_TxtBx;
        private System.Windows.Forms.Label UI_Bus_Lbl;
        private System.Windows.Forms.TextBox UI_Bus_TxtBx;
        private System.Windows.Forms.Button UI_FindFactors_Btn;
        private System.Windows.Forms.Label UI_Stage1_Lbl;
        private System.Windows.Forms.TextBox UI_Stage1_TxtBx;
        private System.Windows.Forms.Label UI_Stage2_Lbl;
        private System.Windows.Forms.TextBox UI_Stage2_TxtBx;
        private System.Windows.Forms.CheckBox UI_CalcPer_CheckBx;
        private System.Windows.Forms.Label UI_ResultFreq_Lbl;
        private System.Windows.Forms.TextBox UI_ResultFreq_TxtBx;
        private System.Windows.Forms.Button UI_20MHzBusRate_Btn;
        private System.Windows.Forms.CheckBox UI_16BitMode_CheckBx;
    }
}

