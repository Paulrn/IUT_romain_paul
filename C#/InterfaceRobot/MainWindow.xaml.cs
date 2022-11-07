using System;
using System.Collections.Generic;
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
        public MainWindow()
        {
            InitializeComponent();
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

            emissionTextBox.SelectionStart = 0;
            emissionTextBox.SelectionLength = 20;
            string selectedText = emissionTextBox.SelectedText;

            emissionTextBox.Text = "";
            receptionTextBox.Text += "Recu : " + selectedText + "\n";
        }

        private void emissionTextBox_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                emissionTextBox.SelectionStart = 0;
                emissionTextBox.SelectionLength = 20;
                string selectedText = emissionTextBox.SelectedText;

                emissionTextBox.Text = "";
                receptionTextBox.Text += "Recu : " + selectedText + "\n";
            }
        }
    }
}