#!packages/FAKE/tools/FAKE.exe
#r @"packages/FAKE/tools/FakeLib.dll"
open Fake
open Fake.FscHelper

RestorePackages()

let buildDir = "./build/"

Target "Clean" (fun _ ->
  CleanDirs [buildDir]
)

Target "BuildAll" (fun _ ->
  !! "src/**/*.fs"
  |> Fsc (fun p ->
           { p with Output = "program.dll"
                    FscTarget = Library })
)

Target "Test" (fun _ ->
  !! (buildDir + "/*.dll")
    |> NUnit (fun p ->
        {p with
           DisableShadowCopy = true;
           OutputFile = testDir + "TestResults.xml" })
)

Target "Default" (fun _ ->
  trace "Default Target."
)

"Clean"
  ==> "BuildAll"
  ==> "Test"
  ==> "Default"

RunTargetOrDefault "Default"
