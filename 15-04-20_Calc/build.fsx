#!packages/FAKE/tools/FAKE.exe
#r @"packages/FAKE/tools/FakeLib.dll"
open Fake

// still does not work
// RestorePackages()

let buildDir = "./build/"
let testDir  = "./test/"

Target "Clean" (fun _ ->
  CleanDirs [buildDir; testDir]
)

Target "BuildApp" (fun _ ->
  !! "src/calc/**/*.fsproj"
    |> MSBuildRelease buildDir "Build"
    |> Log "AppBuild-Output: "
)

Target "BuildGui" (fun _ ->
  !! "src/fgui/**/*.fsproj"
    |> MSBuildRelease buildDir "Build"
    |> Log "AppBuild-Output: "
)

Target "BuildTest" (fun _ ->
  !! "src/test/**/*.fsproj"
    |> MSBuildDebug testDir "Build"
    |> Log "BuildTest-Output: "
)

Target "Test" (fun _ ->
  !! (testDir + "/*.dll")
    |> NUnit (fun p ->
        {p with
           DisableShadowCopy = true;
           OutputFile = testDir + "TestResults.xml" })
)

Target "Default" (fun _ ->
  trace "Default Target."
)

"Clean"
  ==> "BuildApp"
  ==> "BuildGui"
  ==> "BuildTest"
  ==> "Test"
  ==> "Default"

RunTargetOrDefault "Default"
