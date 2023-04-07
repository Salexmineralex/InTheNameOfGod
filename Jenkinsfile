pipeline {
  agent {
    node {
      label 'principal'
      customWorkspace "F:\\CITestingWorkspace"//use backward slashes to avoid problems with how Windows uses directories!!
    }
  }//^all this is necessary to run the build in a special workspace.
  environment {
    ue4Path = "F:\\UE_5.1"
    ue4Project = "InTheNameOfGod"
    ueProjectFileName = "${ue4Project}.uproject"
    testSuiteToRun = "Game."//the '.' is used to run all tests inside the prettyname. The automation system searches for everything that has 'Game.' in it, so otherGame.'s tests would run too...
    testReportFolder = "TestsReport"
    testsLogName = "RunTests.log"
    pathToTestsLog = "${env.WORKSPACE}" + "\\Saved\\Logs\\" + "${testsLogName}"
    codeCoverageReportName="CodeCoverageReport.xml"
  }
  stages {
    stage('Building') {
      steps {
        echo 'Build Stage Started.'
       
       discordSend webhookURL: "https://discord.com/api/webhooks/1093807922124959764/lDjxAT15dNYyoTXhZaptmKHUTAiR1c01HF7H-PRiVx1EjN9mMr-SGw-N1IVftSv038ej",
            title: "${env.JOB_BASE_NAME} #${env.BUILD_NUMBER}",
            result: currentBuild.currentResult,
            description: "**Build:** ${env.BUILD_NUMBER}\n**Status:** ${result}\n\u2060", /* word joiner character forces a blank line */
            enableArtifactsList: true,
            showChangeset: true
        bat "BuildWithoutCooking.bat \"${ue4Path}\" \"${env.WORKSPACE}\" \"${ueProjectFilename}\""//builds our project
      }
      post {
        success {
          echo 'Build Stage Successful.'
        }
        failure {
          echo 'Build Stage Unsuccessful.'
        }
      }
    }

    stage('Testing') {
      steps {
        echo 'Testing Stage Started.'

        bat "TestRunnerAndCodeCoverage.bat \"${ue4Path}\" \"${env.WORKSPACE}\" \"${ueProjectFilename}\" \"${testSuiteToRun}\" \"${testReportFolder}\" \"${testsLogName}\" \"${codeCoverageReportName}\""//runs the tests
      }
      post {
        success {
          echo 'Testing Stage Successful.'
        }
        failure {
          echo 'Testing Stage Unsuccessful.'
        }
      }
    }


  }
  post {
    always{
      echo 'Tests finished, printing log.'
      bat "type ${pathToTestsLog}"
      echo 'Formatting TestsReport from JSon to JUnit XML'
      formatUnitTests()
      
      echo "Publish Code Coverage Report."
      cobertura(coberturaReportFile:"${codeCoverageReportName}")

      echo 'Cleaning up workspace:'
      echo '-checking current workspace.'
      powershell label: 'show workspace', script: 'dir $WORKSPACE'
      bat 'git reset --hard'//resets to HEAD, to the commit in the cloned repository.
      bat 'git clean -dffx .'//removes untracked files.
      echo '-checking clean workspace.'
      powershell label: 'show workspace', script: 'dir $WORKSPACE'

      //^echo 'Sending build status notification to Slack:'
    }
    success{
      echo "success"
 
    }
    unstable{
      echo "unesatable"

    }
    failure{
       echo "failure"
    }
  }
}

import groovy.json.JsonSlurper
import groovy.xml.MarkupBuilder

def testReportSummary = 'to be populated...'

def formatUnitTests() {
        convertTestsReport()
        testReportSummary = junit "${testReportFolder}\\junit.xml"
}

def convertTestsReport() {
       def jsonReport = readFile file: "${testReportFolder}\\index.json", encoding: "UTF-8"
       jsonReport = jsonReport.replace( "\uFEFF", "" );
   
       def xmlContent = transformReport( jsonReport )

       writeFile file: "${testReportFolder}\\junit.xml", text: xmlContent.toString()
   }

@NonCPS//atomic method
def transformReport( String jsonContent ) {

    def parsedReport = new JsonSlurper().parseText( jsonContent )
    
    def jUnitReport = new StringWriter()
    def builder = new MarkupBuilder( jUnitReport )

    builder.doubleQuotes = true
    builder.mkp.xmlDeclaration version: "1.0", encoding: "utf-8"

    builder.testsuite( tests: parsedReport.succeeded + parsedReport.failed, failures: parsedReport.failed, time: parsedReport.totalDuration ) {
      for ( test in parsedReport.tests ) {
        builder.testcase( name: test.testDisplayName, classname: test.fullTestPath, status: test.state ) {
          if(test.state == "Fail") {
            for ( entry in test.entries ) { 
              if(entry.event.type == "Error") {
                builder.failure( message: entry.event.message, type: entry.event.type, entry.filename + " " + entry.lineNumber )
              }
            }
          }
        }
      }
    } 

    return jUnitReport.toString()
}
