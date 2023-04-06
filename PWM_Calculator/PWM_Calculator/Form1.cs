/*
    Author: Andrew Belter
    Creation Date: Oct. 24, 2022
    This program is intended to help find the values for ClockA/B and ClockSA/SB for the PWM module on the 9S12 microcontroller used in CNT.
    The program mostly assumes that the user will input valid values that can be worked with and that the user knows the limitations of the module.
*/

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PWM_Calculator
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void UI_FindFactors_Btn_Click(object sender, EventArgs e)
        {
            //extract and validate the bus rate
            bool validBus = int.TryParse(UI_Bus_TxtBx.Text, out int busRate);
            if (!validBus)
            {
                MessageBox.Show("Invalid bus rate entered. Check value and try again.");
                return;
            }

            //extract and validate the frequency
            bool validFreq = int.TryParse(UI_Freq_TxtBx.Text, out int frequency);
            if (!validFreq)
            {
                MessageBox.Show("Invalid frequency entered. Check value and try again.");
                return;
            }

            if (UI_CalcPer_CheckBx.Checked)
            {
                int bitMode = UI_16BitMode_CheckBx.Checked ? 16 : 8;
                for (int period = (int)Math.Pow(2, bitMode) - 2; period > 0; period -= 2)//subract by 2 to keep period an even number
                {
                    //test all possible combinations of prescales until valid values are found, then stop
                    if (CheckStages(busRate, frequency, period))
                        break;
                }
            }
            else
            {
                //extract and validate the period
                bool validPer = int.TryParse(UI_Per_TxtBx.Text, out int period);
                if (!validPer)
                {
                    MessageBox.Show("Invalid period entered. Check value and try again.");
                    return;
                }

                //display a message telling the user to use 16-bit mode if period > 16
                if (period > 255 && !UI_16BitMode_CheckBx.Checked)
                {
                    MessageBox.Show("Period entered > 255 => Must run the PWM in 16-bit mode.");
                    return;
                }

                //test all possible combinations of prescales until valid values are found, then stop
                if (CheckStages(busRate, frequency, period))
                    return;
            }

            //if this point is reached, no ideal factors were found and a message is displayed to indicate this
            MessageBox.Show("No ideal factors were found. The values shown are the best-fit factors.");

        }

        private bool CheckStages(int busRate, int frequency, int period)
        {
            int bestFitStage1 = 0, bestFitStage2 = 0, bestFitFrequency = 0; 

            double desiredProduct = (double)busRate / (period * frequency);
            for (int stage1 = 0; stage1 <= 7; ++stage1)
            {
                //check if stage2 is needed to get the correct value
                //if the desired product can be achieved with only stage1, we don't need to use stage2                
                if (Math.Pow(2, stage1) == desiredProduct)
                {
                    int checkFreq = busRate / (period * (int)Math.Pow(2, stage1));
                    if (checkFreq == frequency)
                    {
                        UI_Stage1_TxtBx.Text = $"PrescaleStage1_{Math.Pow(2, stage1)}";
                        UI_Stage2_TxtBx.Text = "0 - (Won't use)";
                        UI_Per_TxtBx.Text = $"{period}";
                        UI_ResultFreq_TxtBx.Text = $"{checkFreq}";
                        return true;
                    }
                }

                for (int stage2 = 1; stage2 <= 256; ++stage2)
                {
                    //tempProduct = (2^stage1) * (stage2*2)
                    int tempProduct = stage2 * 2 * (int)Math.Pow(2, stage1);

                    //System.Diagnostics.Trace.WriteLine($"Temp product : {tempProduct}");//testing line

                    //calculate the frequency that 
                    int tempFrequency = busRate / (period * tempProduct);

                    if (period == 1000)
                        MessageBox.Show($"Temp Freq = {tempFrequency}");

                    //if the ideal factors are found, display them and stop here
                    if (tempFrequency == frequency)
                    {
                        UI_Stage1_TxtBx.Text = $"PrescaleStage1_{Math.Pow(2, stage1)}";
                        UI_Stage2_TxtBx.Text = $"{stage2}";
                        UI_Per_TxtBx.Text = $"{period}";
                        UI_ResultFreq_TxtBx.Text = $"{tempFrequency}";
                        return true;
                    }
                    else
                    {
                        //System.Diagnostics.Trace.WriteLine($"Temp:{Math.Abs(frequency - tempProduct)}");
                        if (Math.Abs(frequency - tempFrequency) < Math.Abs(frequency - bestFitFrequency))
                        {
                            bestFitFrequency = tempFrequency;
                            bestFitStage1 = stage1;
                            bestFitStage2 = stage2;
                        }
                    }
                }
            }

            //if this point is reached, no ideal values were found
            UI_Stage1_TxtBx.Text = $"PrescaleStage1_{Math.Pow(2, bestFitStage1)}";
            UI_Stage2_TxtBx.Text = $"{bestFitStage2}";
            UI_Per_TxtBx.Text = $"{period}";
            UI_ResultFreq_TxtBx.Text = $"{bestFitFrequency}";
            return false;
        }

        private void UI_CalcPer_CheckBx_CheckedChanged(object sender, EventArgs e)
        {
            if (UI_CalcPer_CheckBx.Checked)
            {
                UI_Per_TxtBx.Text = "";
                UI_Per_TxtBx.ReadOnly = true;
            }
            else
                UI_Per_TxtBx.ReadOnly = false;
        }

        private void UI_20MHzBusRate_Btn_Click(object sender, EventArgs e)
        {
            //set the bus rate to 20MHz (commonly used value for bus rate)
            UI_Bus_TxtBx.Text = 20E6.ToString();
        }
    }
}
