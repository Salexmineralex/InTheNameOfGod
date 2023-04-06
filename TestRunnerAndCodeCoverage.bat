set ue4Location=%~1
set workspace=%~2
set projectFilename=%~3
set testSuiteToRun=%~4
set testReportFolder=%~5
set testsLogName=%~6
set codeCoverageFile=%~7

set testRunnerCommand="%ue4Location%\Engine\Binaries\Win64\UE4Editor-cmd.exe" "%workspace%\%projectFilename%" -unattended -nopause -NullRHI -ExecCmds="Automation RunTests" -testexit="Automation Test Queue Empty" -ReportOutputPath="%workspace%\%testReportFolder%" -log -Log=%testsLogName%

C:\OpenCppCoverage64\OpenCppCoverage\OpenCppCoverage.exe   --sources=%workspace%\Source --excluded_sources=\Tests --modules %workspace% --export_type=cobertura:%codeCoverageFile% --working_dir=F:\CITestingWorkspace\ -- %testRunnerCommand%
