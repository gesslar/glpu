/**
 * @file /std/cmd/reporter.c
 * @description Inherited by reporting commands, such as bug, typo, and idea.
 *
 * @created 2024/07/13 - Gesslar
 * @last_modified 2024/07/13 - Gesslar
 *
 * @history
 * 2024/07/13 - Gesslar - Created
 */

#include <daemons.h>
#include <ed.h>

inherit STD_CMD ;
inherit M_LOG ;

protected nomask mixed start_report(object tp) ;
public nomask void finish_report(string text, object tp) ;
public nomask void finish_github(mapping response, object tp) ;
public nomask void get_subject(string subject, object tp) ;

private nomask nosave string report_type = "" ;
private nomask nosave string git_hub_label = "" ;

protected nomask void set_report_type(string type) {
    if(!stringp(type) && !strlen(type))
        error("Bad argument 1 to set_report_type().") ;

    report_type = type ;
}

protected nomask void set_git_hub_label(string label) {
    mapping config = mud_config("GITHUB_REPORTER") ;

    if(!stringp(label) && !strlen(label))
        error("Bad argument 1 to set_git_hub_label().\n") ;

    if(!config)
        error("No configuration found for GITHUB_REPORTER.\n") ;

    if(!of("types", config))
        error("No types found in GITHUB_REPORTER configuration.\n") ;

    if(!of(label, config["types"]))
        error("Invalid label. Available labels: " + implode(config["types"], ", ")) ;

    git_hub_label = label ;
}

mixed main(object tp, string str) {
    return start_report(tp, str) ;
}

mixed start_report(object tp, string subject) {
    if(!strlen(report_type))
        return "Report type not set.\n" ;

    if(!stringp(subject) && !strlen(subject)) {
        tell(tp, "Enter a subject for your "+report_type+" report.\nSubject: ") ;
        input_to("get_subject", tp) ;
    } else {
        tell(tp, "Subject: "+subject+"\n") ;
        get_subject(subject, tp) ;
    }

    return 1 ;
}

public nomask void get_subject(string subject, object tp) {
    object editor ;

    if(!subject || !strlen(subject))
        return tell(tp, "Aborted.\n") ;

    tp->start_edit(
        null,
        assemble_call_back((:finish_report:), tp, subject)
    ) ;
}

public nomask void finish_report(int status, string file, string temp_file, object tp, string subject) {
    string log_file, log_text, gh_text ;
    string text ;

    defer((:rm, temp_file:)) ;

    if(status == ED_STATUS_ABORTED || file_size(temp_file) < 1) {
        if(interactive(tp))
            _info(tp, "Report aborted.") ;
        return ;
    }

    text = read_file(temp_file) ;

    assure_dir(log_dir()) ;
    log_file = log_dir() + "/" + upper_case(report_type) ;

    log_text = sprintf("%s\n%s - %s\nSubject: %s\nLocation: %s\n\n%s\n",
        repeat_string("-", 79),
        ldatetime(),
        capitalize(query_privs(tp)),
        subject,
        file_name(environment(tp)),
        text
    ) ;

    write_file(log_file, log_text) ;

    tell(tp, "Thank you for your "+report_type+" report.\n") ;

    gh_text = sprintf("Reporter: %s\nLocation: %s\n\n%s\n",
        capitalize(query_privs(tp)),
        file_name(environment(tp)),
        text
    ) ;
    if(git_hub_label != "")
        GH_ISSUES_D->create_issue(
            git_hub_label,
            subject,
            gh_text,
            assemble_call_back((:finish_github:), tp)
        ) ;
}

public nomask void finish_github(mapping status, object tp) {
    if(status["code"] == 201)
        tell(tp, "Report submitted to GitHub.\n") ;
    else
        tell(tp, "Error submitting report to GitHub.\n") ;

}
