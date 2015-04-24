using System;
using System.Collections.Generic;
using System.Globalization;
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

using calc;

namespace gui {
	public partial class MainWindow : Window {
		Calculator calc;

		private IFormatProvider CULTURE = CultureInfo.InvariantCulture;
		private const NumberStyles STYLE = NumberStyles.Float;
		private bool opPressedLast = false;

		public MainWindow() {
			InitializeComponent();

			// setting up culture
			System.Threading.Thread.CurrentThread.CurrentCulture = CultureInfo.InvariantCulture;

			// initializing buttons
			bAC.Click += bAC_Click;
			bMR.Click += bMR_Click;
			bMC.Click += bMC_Click;
			bMp.Click += bMp_Click;
			bBs.Click += bBs_Click;
			bSign.Click += bSign_Click;
			b0.Click += bSymbol_Click;
			b1.Click += bSymbol_Click;
			b2.Click += bSymbol_Click;
			b3.Click += bSymbol_Click;
			b4.Click += bSymbol_Click;
			b5.Click += bSymbol_Click;
			b6.Click += bSymbol_Click;
			b7.Click += bSymbol_Click;
			b8.Click += bSymbol_Click;
			b9.Click += bSymbol_Click;
			bDot.Click += bSymbol_Click;
			bPls.Click += bOperation_Click;
			bMns.Click += bOperation_Click;
			bMlt.Click += bOperation_Click;
			bDiv.Click += bOperation_Click;
			bPow.Click += bOperation_Click;
			bEq.Click += bEq_Click;

			calc = new Calculator();

			this.KeyDown += new KeyEventHandler(MainWindow_KeyDown);
		}

		void bAC_Click(object sender, RoutedEventArgs e) {
			doAC();
		}

		void bMR_Click(object sender, RoutedEventArgs e) {
			doMR();
		}

		void bMC_Click(object sender, RoutedEventArgs e) {
			doMC();
		}

		void bMp_Click(object sender, RoutedEventArgs e) {
			doMp();
		}

		void bBs_Click(object sender, RoutedEventArgs e) {
			doBs();
		}

		void bSign_Click(object sender, RoutedEventArgs e) {
			doSign();
		}

		void bSymbol_Click(object sender, RoutedEventArgs e) {
			doSymbol(((Button)sender).Tag.ToString());
		}

		void bOperation_Click(object sender, RoutedEventArgs e) {
			doOperation(((Button)sender).Tag.ToString()[0]);
		}

		void bEq_Click(object sender, RoutedEventArgs e) {
			doCalc();
		}

		private void MainWindow_KeyDown(object sender, KeyEventArgs e) {
			switch (e.Key) {
				case Key.Back:
					doBs();
					break;
				case Key.D0:
				case Key.NumPad0:
					doSymbol("0");
					break;
				case Key.D1:
				case Key.NumPad1:
					doSymbol("1");
					break;
				case Key.D2:
				case Key.NumPad2:
					doSymbol("2");
					break;
				case Key.D3:
				case Key.NumPad3:
					doSymbol("3");
					break;
				case Key.D4:
				case Key.NumPad4:
					doSymbol("4");
					break;
				case Key.D5:
				case Key.NumPad5:
					doSymbol("5");
					break;
				case Key.D6:
				case Key.NumPad6:
					doSymbol("6");
					break;
				case Key.D7:
				case Key.NumPad7:
					doSymbol("7");
					break;
				case Key.D8:
				case Key.NumPad8:
					doSymbol("8");
					break;
				case Key.D9:
				case Key.NumPad9:
					doSymbol("9");
					break;
				case Key.Decimal:
				case Key.OemPeriod:
					doSymbol(".");
					break;
				case Key.Add:
					doOperation('+');
					break;
				case Key.Subtract:
					doOperation('-');
					break;
				case Key.Multiply:
					doOperation('*');
					break;
				case Key.Divide:
					doOperation('/');
					break;
				case Key.Enter:
					doCalc();
					break;
				default:
					break;
			}
		}

		// -------------------------------------------------------------

		void ResetStatus() {
			textBox_Status.Text = "";
		}

		void doAC() {
			calc.Reset();
			ResetStatus();
			textBox_Digits.Text = "0";
		}

		void doMR() {
			textBox_Digits.Text = calc.MemoryRead().ToString();
			calc.DigitPressed();
		}

		void doMC() {
			calc.MemoryClean();
		}

		void doMp() {
			double num = 0.0;
			if (double.TryParse(textBox_Digits.Text.Replace("−", "-"), STYLE, CULTURE, out num)) {
				calc.MemoryAdd(num);
				opPressedLast = true;
			}
			else {
				textBox_Status.Text = "Format error!";
			}
		}

		void doBs() {
			if (textBox_Digits.Text.Length > 1) {
				textBox_Digits.Text = textBox_Digits.Text.Substring(0, textBox_Digits.Text.Length - 1);
			}
			else {
				textBox_Digits.Text = "0";
			}
			ResetStatus();
		}

		void doSign() {
			if ((textBox_Digits.Text[0] != '-') && (!textBox_Digits.Text.Equals("0"))) {
				textBox_Digits.Text = "-" + textBox_Digits.Text;
			}
			else {
				textBox_Digits.Text = textBox_Digits.Text.Substring(1);
			}
		}

		void doSymbol(string s) {
			if (opPressedLast || (textBox_Digits.Text.Equals("0") && !s.Equals("."))) {
				textBox_Digits.Text = s;
				opPressedLast = false;
			}
			else {
				textBox_Digits.Text = textBox_Digits.Text + s;
			}
			calc.DigitPressed();
			ResetStatus();
		}

		void doOperation(char c) {
			double num;
			if (double.TryParse(textBox_Digits.Text, STYLE, CULTURE, out num)) {
				if (calc.Operation(num, c)) {
					opPressedLast = true;
				}
				else {
					textBox_Status.Text = "Invalid operation!";
				}
			}
			else {
				textBox_Status.Text = "Format error!";
			}
		}

		void doCalc() {
			double num;
			if (double.TryParse(textBox_Digits.Text.Replace("−", "-"), STYLE, CULTURE, out num)) {
				var res = calc.Calculate(num);
				if (res.IsInvOp) {
					textBox_Status.Text = "Invalid operation!";
				}
				else if (res.IsDivByZero) {
					textBox_Status.Text = "Division by zero!";
				}
				else {
					textBox_Digits.Text = String.Format("{0:0.###}", ((CalcRes.Ok)res).Item);
					opPressedLast = true;
				}
			}
			else {
				textBox_Status.Text = "Format error!";
			}
		}
	}
}
