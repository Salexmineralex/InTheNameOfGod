set ue4Location=%~1
set workspace=%~2
set projectFilename=%~3
set testSuiteToRun=%~4
set testReportFolder=%~5
set testsLogName=%~6
set codeCoverageFile=%~7

//
F:\UE_5.1\Engine\Binaries\Win64\UnrealEditor-Cmd.exe "F:\CITestingWorkspace\InTheNameOfGod.uproject" -unattended -nopause -NullRHI -ExecCmds="Automation RunTests" -testexit="Automation Test Queue Empty" -log -log=RunTests.log -ReportOutputPath="F:\CITestingWorkspace\Saved\UnitTestsReport"
C:\OpenCppCoverage64\OpenCppCoverage\OpenCppCoverage.exe   --sources=%workspace%\Source --excluded_sources=\Tests --modules %workspace% --export_type=cobertura:%codeCoverageFile% --working_dir=F:\CITestingWorkspace\ -- %testRunnerCommand%
