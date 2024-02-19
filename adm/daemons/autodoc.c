// /adm/daemons/autodoc.c
// This is Gesslar's attempt at an autoducumentation system.
//
// Created:     2024/02/18: Gesslar
// Last Change: 2024/02/18: Gesslar
//
// 2024/02/18: Gesslar - Created

inherit STD_DAEMON ;

private nosave float dir_delay = 0.02 ;
private nosave float file_delay = 0.01 ;

private nosave int start_time_ms = 0 ;

private nosave mixed *dirs_to_check = ({}) ;
private nosave mixed *files_to_check = ({}) ;

private nosave int dir_call_out = 0 ;
private nosave int file_call_out = 0 ;

int check_running() ;
void finish_scan() ;
void parse_file(string *file) ;

void autodoc_scan() {
    mixed *config ;

    if(check_running() == TRUE)
        return ;

    start_time_ms = time_ns() / 1_000_000 ;
    dirs_to_check = mud_config("AUTODOC_DIRS") ;

    dir_call_out = call_out_walltime("check_dir", dir_delay) ;
}

void check_dir() {
    mixed *current ;
    string *files ;
    string source_dir, dest_dir ;
    string orig ;

    if(check_running() == FALSE)
        return finish_scan() ;

    current = dirs_to_check[0] ;
    source_dir = current[0] ;
    dest_dir = current[1] ;

    mkdirs(dest_dir) ;

    source_dir = append(source_dir, "/") ;
    files = get_dir(append(source_dir, "*.c")) ;
    files = map(files, (: $2+$1 :), source_dir) ;
    foreach(string file in files)
        files_to_check += ({ ({ file, dest_dir }) }) ;

    file_call_out = call_out_walltime("check_file", file_delay) ;
    dirs_to_check = dirs_to_check[1..] ;
    if(sizeof(dirs_to_check) == 0)
        dir_call_out = 0 ;
    else
        dir_call_out = call_out_walltime("check_dir", dir_delay) ;
}

void check_file() {
    string *file ;

    if(check_running() == FALSE)
        return finish_scan() ;

    file = files_to_check[0] ;
    files_to_check = files_to_check[1..] ;

    if(sizeof(files_to_check) == 0)
        file_call_out = 0 ;
    else
        file_call_out = call_out_walltime("check_file", file_delay) ;

    parse_file(file) ;
}

void parse_file(string *file_info) {
    string *lines ;
    string file, dest_dir ;
    int num, max ;

    file = file_info[0] ;
    dest_dir = file_info[1] ;

    if(!file_exists(file))
        return ;

    lines = explode(read_file(file), "\n") ;

    printf("Parsing file: %s to dir %s\n", file, dest_dir) ;
    max = sizeof(lines) ;
    num = 0 ;

    for(;num < max;num++) {
        string line ;
        int curr ;
        string tag, info ;
        string out = "" ;
        int stop ;

        curr++ ;
        line = lines[num] ;
        if(pcre_match(line, "^//:") == TRUE) {
            line = line[3..] ;
    }
}

void finish_scan() {
    debug_message("Autodoc scan finished in " + (time_ns() / 1_000_000 - start_time_ms) + "ms.") ;
}

int check_running() {
    return sizeof(dirs_to_check) > 0 || sizeof(files_to_check) > 0 ;
}
