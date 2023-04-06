set ue4Location=%~1
set workspace=%~2
set projectFilename=%~3
set testSuiteToRun=%~4
set testReportFolder=%~5
set testsLogName=%~6
set codeCoverageFile=%~7

//set testRunnerCommand="%ue4Location%\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" "%workspace%\%projectFilename%" -nosplash -Unattended -nopause  -NullRHI -nocontentbrowser  -ExecCmds="Automation RunTests %testSuiteToRun%;quit" -TestExit="Automation Test Queue Empty" -ReportOutputPath="%workspace%\%testReportFolder%" -log -Log=%testsLogName%

set testRunnerCommand="%ue4Location%\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" "%workspace%\%projectFilename%" -nosplash -Unattended -nopause  -NullRHI -nocontentbrowser  ExecCmds="Automation RunTests;quit" -TestExit="Automation Test Queue Empty" -ReportOutputPath="%workspace%\%testReportFolder%" -log -Log=%testsLogName%

C:\OpenCppCoverage64\OpenCppCoverage\OpenCppCoverage.exe   --sources=%workspace%\Source --excluded_sources=\Tests --modules %workspace% --export_type=cobertura:%codeCoverageFile% --working_dir=F:\CITestingWorkspace\ -- %testRunnerCommand%
