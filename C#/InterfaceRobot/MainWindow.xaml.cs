
using ExtendedSerialPort;
using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace InterfaceRobot
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        ReliableSerialPort serialPort1;

        public MainWindow()
        {
            InitializeComponent();
            serialPort1 = new ReliableSerialPort("COM6", 115200, Parity.None, 8, StopBits.One);
            serialPort1.Open();
        }

        int couleur = 0;
        private void buttonEnvoyer_Click(object sender, RoutedEventArgs e)
        {
            couleur++; couleur = couleur % 5;

            switch (couleur)
            {
                case (0): buttonEnvoyer.Background = Brushes.RoyalBlue; break;
                case (1): buttonEnvoyer.Background = Brushes.Purple; break;
                case (2): buttonEnvoyer.Background = Brushes.Red; break;
                case (3): buttonEnvoyer.Background = Brushes.Orange; break;
                case (4): buttonEnvoyer.Background = Brushes.Beige; break;
            }

            SendMessage();

        }

        private void emissionTextBox_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                SendMessage();
            }
        }

        private void SendMessage()
        {
            //receptionTextBox.Text += "Recu : " + emissionTextBox.Text + "\n";
            serialPort1.Write(emissionTextBox.Text);    //ecris sur le port1
            emissionTextBox.Text = "";  //clear la text box emission
        }
    }
}