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

inherit STD_CMD ;
inherit M_LOG ;

protected nomask mixed start_report(object tp) ;
public nomask void finish_report(string text, object tp) ;
public nomask void finish_github(mapping response, object tp) ;

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
    return start_report(tp) ;
}

mixed start_report(object tp) {
    if(!strlen(report_type))
        return "Report type not set.\n" ;

    tell(tp, "Enter a subject for your "+report_type+" report.\nSubject: ") ;
    input_to("get_subject", tp) ;

    return 1 ;
}

public nomask void get_subject(string subject, object tp) {
    object editor ;

    if(!subject || !strlen(subject))
        return tell(tp, "Aborted.\n") ;

    editor = new(OBJ_EDITOR) ;
    editor->edit(tp, null, assemble_call_back((:finish_report:), tp, subject)) ;

    return ;
}

public nomask void finish_report(string text, object tp, string subject) {
    string log_file ;

    if(!text || text == "")
        return tell(tp, "Aborted.\n") ;

    tell(tp, "Thank you for your "+report_type+" report.\n") ;

    assure_dir(log_dir()) ;
    log_file = log_dir() + "/" + upper_case(report_type) ;

    write_file(log_file,
        sprintf("%s\n%s\n%s\n%s\n",
            ldatetime(),
            subject,
            query_privs(tp),
            text
        )
    ) ;

    if(git_hub_label != "")
        GH_ISSUES_D->create_issue(
            git_hub_label,
            subject,
            text,
            assemble_call_back((:finish_github:), tp)
        ) ;
}

public nomask void finish_github(mapping status, object tp) {
    if(status["code"] == 201)
        tell(tp, "Report submitted to GitHub.\n") ;
    else
        tell(tp, "Error submitting report to GitHub.\n") ;

}
