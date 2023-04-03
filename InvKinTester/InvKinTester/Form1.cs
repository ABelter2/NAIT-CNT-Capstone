using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace InvKinTester
{
    public partial class Form1 : Form
    {
        // inverse kinematics constants (mm)
        const double EffectorRadius = 35;
        const double BaseRadius = 100;
        const double ForearmLength = 280;
        const double BicepLength = 285;
        const double BaseToFloor = 200;

        static double sqrt3 = Math.Sqrt(3);
        static double pi = Math.PI;
        static double sin120 = sqrt3 / 2.0;
        static double cos120 = -0.5;
        static double tan60 = sqrt3;
        static double sin30 = 0.5;
        static double tan30 = 1.0 / sqrt3;

        public Form1()
        {
            InitializeComponent();
        }

        // returns 0 for success, -1 for error
        int Cap_CalcAngleXYZ(double x0, double y0, double z0, out double theta)
        {
            double y1 = -0.5 * tan30 * BaseRadius;
            y0 -= 0.5 * tan30 * EffectorRadius; // shift center to edge

            double aV = (x0 * x0 + y0 * y0 + z0 * z0 + BicepLength * BicepLength - ForearmLength * ForearmLength - y1 * y1) / (2.0 * z0);
            double bV = (y1 - y0) / z0;

            // discriminant
            double dV = -(aV + bV * y1) * (aV + bV * y1) + BicepLength * (bV * bV * BicepLength + BicepLength);
            if (dV < 0) // non-exisiting power - return error -1, theta = 0
            {
                theta = 0;
                return -1;
            }

            double yj = (y1 - aV * bV - Math.Sqrt(dV)) / (bV * bV + 1);
            double zj = aV + bV * yj;
            theta = Math.Atan2(-zj, y1 - yj) * 180.0 / pi;
            //theta = Math.Atan(-zj / (y1 - yj)) * 180 / pi + ((yj > y1) ? 180.0 : 0.0);

            return 0;
        }

        // returns 0 for success, -# for error - # is what step it failed on
        int Cap_CalcInvKin(double[] thetas, double x0, double y0, double z0)
        {
            double theta1, theta2, theta3;

            if (Cap_CalcAngleXYZ(x0, y0, z0, out theta1) != 0)
                return -1;
            thetas[0] = theta1;

            if (Cap_CalcAngleXYZ(x0 * cos120 + y0 * sin120, y0 * cos120 - x0 * sin120, z0, out theta2) != 0)
                return -2;
            thetas[1] = theta2;

            if (Cap_CalcAngleXYZ(x0 * cos120 - y0 * sin120, y0 * cos120 + x0 * sin120, z0, out theta3) != 0)
                return -3;
            thetas[2] = theta3;

            return 0;
        }

        private void Btn_Calculate_Click(object sender, EventArgs e)
        {
            double[] thetas = new double[3];
            Cap_CalcInvKin(thetas, (double)Num_X.Value, (double)Num_Y.Value, (double)Num_Z.Value);

            Txt_Theta1.Text = $"{thetas[0]:F5}";
            Txt_Theta2.Text = $"{thetas[1]:F5}";
            Txt_Theta3.Text = $"{thetas[2]:F5}";
        }
    }
}
