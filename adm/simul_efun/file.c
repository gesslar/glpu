#include "/adm/obj/simul_efun.h"

#include <config.h>

void assure_file(string file) {
    string *path;
    string dir;
    int i;

    if(file_size(file) != -1) return;

    path = explode(file, "/");
    dir = "";

    for(i = 0; i < sizeof(path) - 1; i++) {
        dir += "/" + path[i];
        if(file_size(dir) == -1) mkdir(dir);
    }
}

//file_owner(string file);

//Tacitus @ LPUniversity
//02-APR-05
//Simul-Efun

//Last edited on July 1st, 2005 by Tacitus

string file_owner(string file) {
    string temp;

    if(file[0] != '/') file = "/" + file;

    if(sscanf(file, "/home/%s/%s/%*s", temp, temp) == 2) {
        return temp;
    }
    if(sscanf(file, "/adm/%s/%*s", temp) == 2) {
        return "Admin/" + temp;
    }

    return 0;
}

//file_lines(string file);

//Gwegster @ LPUniversity
//30-JUNE-05
// A simul_efun that takes the specified file and checks for
//      the amount of "\n"s in the file, thereby getting the
//      total amount of lines in the file.

int file_lines(string file) {
    string str;
    string *lines;

    if(file_size(file) < 1)
    return 0;

    str = read_file(file);
    lines = explode(str + "", "\n");
    return sizeof(lines);
}

varargs string tail(string path, int line_count) {
    int chunk_size = 80 * 5; // Chunk size for each read operation
    string result = ""; // Accumulator for the result
    int total_lines = 0; // Counter for total lines found
    int file_size ; // Total size of the file
    int start, end; // Variables to define the read range
    int read_chars = 0; // Counter for characters read without finding a newline
    int newlines_found = 0; // Counter for newlines found in the current chunk

    if(nullp(path)) error("No file specified for tail(). [" + previous_object() + "]");
    if(nullp(line_count)) line_count = 25 ; // Default to 25 lines if not specified

    // Get the total size of the file
    file_size = file_size(path);
    // Initialize reading position at the end of the file
    end = file_size;

    // Ensure we don't start reading beyond the start of the file
    if (end < 0) return "File does not exist or is empty.";

    while (total_lines < line_count && end > 0) {
        string chunk; // Variable to hold the data read in each chunk
        start = end - chunk_size;
        if (start < 0) start = 0; // Adjust start to not go below file beginning

        // Read the chunk from the file
        chunk = read_bytes(path, start, end - start);

        if (!strlen(chunk)) break; // Break if no data was read

        // Iterate through the chunk to count newlines
        for (int i = strlen(chunk) - 1; i >= 0; i--) {
            if (chunk[i] == '\n') {
                newlines_found++;
                if (newlines_found >= line_count) break; // Stop if we have enough lines
            }
            read_chars++;

            // Check if we've read too many characters without a newline
            if (read_chars > 2000) break;
        }

        // Prepend the current chunk to the result
        result = chunk + result;

        // Adjust counters based on what was found
        total_lines += newlines_found;
        end = start; // Move the end position for the next read

        // Reset counters for the next iteration
        read_chars = 0;
        newlines_found = 0;

        // Break early if we've read too many characters without finding a newline
        if (read_chars > 2000) break;
    }

    // Trim the result if we've collected more lines than needed
    if (total_lines > line_count) {
        int lines_to_trim = total_lines - line_count;
        int pos = 0;
        for (int i = 0; i < strlen(result) && lines_to_trim > 0; i++) {
            if (result[i] == '\n') {
                lines_to_trim--;
                pos = i + 1;
            }
        }
        result = result[pos..]; // Keep only the required number of lines from the end
    }

    return result;
}


//log_file(string file, string str)

//Tacitus @ LPUniversity
//15-APR-05
//Simul-efuns

varargs int log_file(string file, string str, mixed arg...) {
    if(!file || !str) return 0;
    master()->log_file(file, str, arg...);
    return 1;
}

string *explode_file(string file) {
    string old_privs ;

    if(!file) return ({}) ;

    old_privs = query_privs() ;
    set_privs(this_object(), query_privs(previous_object())) ;

    if(!file_exists(file)) {
        set_privs(this_object(), old_privs) ;
        return ({}) ;
    }

    catch {
        string data = read_file(file) ;
        string *lines ;

        if(!data) {
            set_privs(this_object(), old_privs) ;
            return ({}) ;
        }
        lines = explode(data, "\n") ;
        lines = filter(lines, (: $1[0] != '#' :)) ;
        lines = filter(lines, (: strlen(trim($1)) > 0 :)) ;
    } ;

    set_privs(this_object(), old_privs) ;

    return lines ;
}

varargs void implode_file(string file, string *lines, int overwrite) {
    string old_privs ;

    if(!file) return ;
    if(!lines) return ;
    if(!sizeof(lines)) return ;

    overwrite = !!overwrite ;
    old_privs = query_privs() ;
    set_privs(this_object(), query_privs(previous_object())) ;

    catch {
        write_file(file, implode(lines, "\n") + "\n", overwrite) ;
    } ;

    set_privs(this_object(), old_privs) ;
}

string query_file_name(object ob) {
    string file, *parts;
    string dir ;

    if(!objectp(ob))
        ob = previous_object() ;

    if(!objectp(ob))
        error("Bad argument 1 to query_file_name().\n") ;

    file = base_name(ob) ;
    parts = explode(file, "/");
    file = parts[<1];

    return file;
}
