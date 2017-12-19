from jira.client import JIRA
server='https://dvsdev.jira.com'
username='AngryRobot'
password='welcome000'

def displayJiraTicket(issue):
    print '----------------------------';
    print "JIRA_KEY: "+str(issue.fields.project.key)
    print "Issuetype: "+str(issue.fields.issuetype.name)
    print "Reporter: "+str(issue.fields.reporter.displayName)
    print "Assignee: "+str(issue.fields.assignee)
    print "summary: "+str(issue.fields.summary)
    print "Status: "+str(issue.fields.status)
    print "Description: "+str(issue.fields.description)
    print "Priority: "+str(issue.fields.priority)
    print "Resolution: "+str(issue.fields.resolution)
    print '----------------------------'
    return

def ReadJiraTicket(jiraName):
    jira = JIRA(server,basic_auth=(username,password));
    issue = jira.issue(jiraName);
    displayJiraTicket(issue);
    return

def UpdateJiraTicket(jiraName,summary,description,priorityNum):
    jira = JIRA(server,basic_auth=(username,password));
    issue = jira.issue(jiraName);
    issue.update(fields={'summary': summary, 'description': description ,'priority':{'id': priorityNum}});
    return

def CommentJira(jiraID,comment):
    jira = JIRA(server,basic_auth=(username,password));
    issue = jira.issue(jiraName);
    jira.add_comment(issue, 'Comment text')
    return

def ReAssignJiraTicket(jiraName,assignID):
    jira = JIRA(server,basic_auth=(username,password));
    issue = jira.issue(jiraName);
    jira.assign_issue(issue, assignID)
    return

#

TestResult=0;
jiraIssueID='TESTRY-6';
resultFlag='.FAILED';
jiraTitleID='testcase updated'
summaryName=jiraTitleID+resultFlag;


ReadJiraTicket(jiraIssueID);

if(TestResult==0):
    resultFlag='.PASSED';
    
summaryName=jiraTitleID+resultFlag;

UpdateJiraTicket('TESTRY-6',summaryName,'issue has been updated','3')

ReAssignJiraTicket('TESTRY-6','mle')





