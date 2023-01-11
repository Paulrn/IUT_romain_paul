
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
            string s = "bonjour";
            byte[] payload = Encoding.ASCII.GetBytes(s);
            UartEncodeAndSendMessage(0x0080, 7, payload);

            //byte[] byteList = new byte[20];
            //for (int i = 0; i < 20; i++)
            //{
            //    byteList[i] = (byte)(2 * i);
            //}
            //serialPort1.Write(byteList,0,20);
            //robot.receivedText += "\n";
        }

        private byte calcul_checksum(int msgFunction,int msgPayloadLength, byte[] msgPayload)
        {
            byte checksum = 0x00;
            checksum ^= 0xFE;
            checksum ^= (byte)(msgFunction >> 8);
            checksum ^= (byte)(msgFunction);
            checksum ^= (byte)(msgPayloadLength >> 8);
            checksum ^= (byte)(msgPayloadLength);

            for(int i=0; i< msgPayloadLength; i++)
            {
                checksum ^= msgPayload[i];
            }
            return checksum;
        }

        private void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            byte[] message = new byte[msgPayloadLength + 6];
            int pos = 0;
            
            message[pos++] = 0xFE;
            message[pos++] = (byte)(msgFunction >> 8);
            message[pos++] = (byte)(msgFunction);
            message[pos++] = (byte)(msgPayloadLength >> 8);
            message[pos++] = (byte)(msgPayloadLength);
            for (int i = 0; i < msgPayloadLength; i++)
            {
                message[pos++]= msgPayload[i];
            }
            message[pos++] = calcul_checksum(msgFunction, msgPayloadLength, msgPayload);

            serialPort1.Write(message, 0, pos);
        }

        public enum StateReception
        {
            Waiting,
            FunctionMSB,
            FunctionLSB,
            PayloadLengthMSB,
            PayloadLengthLSB,
            Payload,
            CheckSum
        }

        StateReception rcvState = StateReception.Waiting;
        int msgDecodedFunction = 0;
        int msgDecodedPayloadLength = 0;
        byte[] msgDecodedPayload;
        int msgDecodedPayloadIndex = 0;

        private void DecodeMessage(byte c)
        {
            switch (rcvState)
            {
                case StateReception.Waiting:
                    if (c == 0xFE)
                        rcvState = StateReception.FunctionMSB;
                    break;
                case StateReception.FunctionMSB:
                    msgDecodedFunction = ((int)c << 8);
                    rcvState = StateReception.FunctionLSB;
                    break;
                case StateReception.FunctionLSB:
                    msgDecodedFunction += ((int)c << 0);
                    rcvState = StateReception.PayloadLengthMSB;
                    break;
                case StateReception.PayloadLengthMSB:
                    msgDecodedPayloadLength = ((int)c << 8);
                    rcvState = StateReception.PayloadLengthLSB;
                    break;
                case StateReception.PayloadLengthLSB:
                    msgDecodedFunction += ((int)c << 0);
                    rcvState = StateReception.Payload;
                    break;
                case StateReception.Payload:
                    msgDecodedPayload[msgDecodedPayloadIndex]= c;
                    msgDecodedPayloadIndex++ ;
                    if(msgDecodedPayloadIndex>msgDecodedPayloadLength)
                    {
                        rcvState = StateReception.CheckSum;
                    }
                    break;
                case StateReception.CheckSum:
                    if (calcul_checksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload) == c)
                    {
                        serialPort1.Write(emissionTextBox.Text);
                        emissionTextBox.Text = "ok";
                        //Success, on a un message valide
                    }
                    
                    break;
                default:
                    rcvState = StateReception.Waiting;
                    break;
            }
        }

        void ProcessDecodedMessage(int msgFunction,int msgPayloadLength, byte[] msgPayload)
        {

        }


    }