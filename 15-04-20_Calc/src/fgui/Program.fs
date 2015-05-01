open System
open System.Drawing
open System.Globalization
open System.Windows.Forms

open calc

let CULTURE = CultureInfo.InvariantCulture
let STYLE = NumberStyles.Float
let clc = new Calculator()
let mutable opPressedLast = false

let textBox_Digits =
  let textBox = new TextBox()
  textBox.Location <- Point(5, 5)
  textBox.Multiline <- false
  textBox.Height <- 40
  textBox.Width  <- 175
  textBox.Font <- new Font("Courier New", 16.0f)
  textBox.BackColor <- Color.White
  textBox.TextAlign <- HorizontalAlignment.Right
  textBox.ReadOnly <- true
  textBox.Text <- "0"
  textBox

let textBox_Status =
  let label = new Label()
  label.Location <- Point(5, 50)
  label.Height <- 30
  label.Width  <- 175
  label.Font <- new Font("Arial", 12.0f)
  label.Focus() |> ignore
  label

let makeButton row col colspan text action =
  let button = new Button()
  button.Location <- Point(5 + col * 45, 80 + row * 45)
  button.Height <- 40
  button.Width <- 40 * colspan + 5 * (colspan - 1)
  button.Text <- text
  button.Click.Add action
  button

// ---------------------------------------------------------------------------
// Actions

let doAC _ =
  clc.Reset()
  textBox_Status.Text <- ""
  textBox_Digits.Text <- "0"

let doMR _ =
  textBox_Digits.Text <- clc.MemoryRead().ToString()
  clc.DigitPressed()

let doMC _ =
  clc.MemoryClean()

let doMp _ =
  match Double.TryParse(textBox_Digits.Text.Replace("−", "-"), STYLE, CULTURE) with
  | true, num ->
    clc.MemoryAdd num
    opPressedLast <- true
  | false, _ ->
    textBox_Status.Text <- "Format error!"

let doBs _ =
  if (textBox_Digits.Text.Length > 1) then
    textBox_Digits.Text <- textBox_Digits.Text.Substring(0, textBox_Digits.Text.Length - 1)
  else
    textBox_Digits.Text <- "0"
  textBox_Status.Text <- ""

let doSign _ =
  if ((textBox_Digits.Text.[0] <> '-') && (not (textBox_Digits.Text.Equals("0")))) then
    textBox_Digits.Text <- "-" + textBox_Digits.Text
  else
    textBox_Digits.Text <- textBox_Digits.Text.Substring(1)

let doSymbol s _ =
  if (opPressedLast || (textBox_Digits.Text.Equals("0") && not (s.Equals(".")))) then
    textBox_Digits.Text <- s.ToString()
    opPressedLast <- false
  else
    textBox_Digits.Text <- textBox_Digits.Text + s.ToString()
  clc.DigitPressed()
  textBox_Status.Text <- ""

let doOperation c _ =
  match Double.TryParse(textBox_Digits.Text.Replace("−", "-"), STYLE, CULTURE) with
  | true, num ->
      if (clc.Operation num c) then
          opPressedLast <- true
      else
          textBox_Status.Text <- "Invalid op! " + num.ToString() + " " + c.ToString()
  | false, _ ->
      textBox_Status.Text <- "Format error!"

let doCalc _ =
  match Double.TryParse(textBox_Digits.Text.Replace("−", "-"), STYLE, CULTURE) with
  | true, num ->
    match clc.Calculate(num) with
    | InvOp ->
      textBox_Status.Text <- "Invalid operation!"
    | DivByZero ->
      textBox_Status.Text <- "Division by zero!"
    | Ok(r) ->
      textBox_Digits.Text <- String.Format("{0:0.###}", r)
      opPressedLast <- true
  | false, _ ->
    textBox_Status.Text <- "Format error!"

// ---------------------------------------------------------------------------
// Initialization

let mainForm =
  let form = new Form(Visible = false, TopMost = true)
  form.MaximizeBox <- false 
  form.MaximumSize <- Size(205, 400)
  form.MinimumSize <- Size(205, 400)
  form.Text <- "MATMEX Calc"
  form.BackColor <- Color.LightGray

  form.Controls.Add(textBox_Digits)
  form.Controls.Add(textBox_Status)
  form.Controls.Add(makeButton 0 0 1 "AC" doAC)
  form.Controls.Add(makeButton 0 1 1 "MR" doMR)
  form.Controls.Add(makeButton 0 2 1 "MC" doMC)
  form.Controls.Add(makeButton 0 3 1 "M+" doMp)
  form.Controls.Add(makeButton 1 0 1 "←" doBs)
  form.Controls.Add(makeButton 1 1 1 "±" doSign)
  form.Controls.Add(makeButton 1 2 1 "^" (doOperation '^'))
  form.Controls.Add(makeButton 1 3 1 "/" (doOperation '/'))
  form.Controls.Add(makeButton 2 3 1 "*" (doOperation '*'))
  form.Controls.Add(makeButton 3 3 1 "-" (doOperation '-'))
  form.Controls.Add(makeButton 4 3 1 "+" (doOperation '+'))
  for i in 1..9 do
    form.Controls.Add(
      makeButton (4 - (i - 1) / 3) ((i - 1) % 3) 1 (i.ToString()) (doSymbol (char (48 + i))))
  form.Controls.Add(makeButton 5 0 2 "0" (doSymbol '0'))
  form.Controls.Add(makeButton 5 2 1 "." (doSymbol '.'))
  form.Controls.Add(makeButton 5 3 1 "=" doCalc)
  form

[<EntryPoint>]
let main argv = 
  mainForm.Visible <- true
  Application.Run()
  0