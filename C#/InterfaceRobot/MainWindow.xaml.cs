
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
using System.Windows.Threading;

namespace InterfaceRobot
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        ReliableSerialPort serialPort1;
        DispatcherTimer timerAffichage;
        Robot robot = new Robot();

        public MainWindow()
        {
            InitializeComponent();
            serialPort1 = new ReliableSerialPort("COM6", 115200, Parity.None, 8, StopBits.One);
            serialPort1.Open();

            serialPort1.DataReceived += SerialPort1_DataReceived;

            timerAffichage = new DispatcherTimer();
            timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 100);
            timerAffichage.Tick += TimerAffichage_Tick; ;
            timerAffichage.Start();
        }

        private void SerialPort1_DataReceived(object sender, DataReceivedArgs e)
        {
            robot.receivedText += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);
            foreach (var c in e.Data)
            {
                robot.byteListReceived.Enqueue(c);
            }
        }

        private void TimerAffichage_Tick(object sender, EventArgs e)
        {
            while(robot.byteListReceived.Count() > 0)
            {
                var c = robot.byteListReceived.Dequeue();
                receptionTextBox.Text += "0x" + c.ToString("X2") + " ";                
            }
            if (robot.receivedText != "")
            {
                receptionTextBox.Text += robot.receivedText;                
                robot.receivedText = "";
            }
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
            robot.receivedText = "\n";
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

        private void buttonClear_Click(object sender, RoutedEventArgs e)
        {
            receptionTextBox.Text = "";
        }

        private void test_Click(object sender, RoutedEventArgs e)
        {
            byte[] byteList = new byte[20];
            for (int i = 0; i < 20; i++)
            {
                byteList[i] = (byte)(2 * i);
            }
            serialPort1.Write(byteList,0,20);
            robot.receivedText += "\n";
        }
    }
}