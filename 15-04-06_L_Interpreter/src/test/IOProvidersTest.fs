module IOProvidersTest

open NUnit.Framework
open IOProviders

[<Test>]
let listInProviderTest() =
  let ip = new ListInputProvider([0; -2147483648; 228]) :> IInputProvider
  Assert.AreEqual(ip.Read(), 0)
  Assert.AreEqual(ip.Read(), -2147483648)
  Assert.AreEqual(ip.Read(), 228)

[<Test>]
let stringOutProviderTest() =
  let op = new StringOutputProvider()
  (op :> IOutputProvider).Write(1)
  (op :> IOutputProvider).Write(2147483647)
  (op :> IOutputProvider).Write(-144);
  Assert.AreEqual(op.GetString(), "1\n2147483647\n-144\n")