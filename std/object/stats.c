// /std/object/stats.c
// Object stats
//
// Created:     2024/02/04: Gesslar
// Last Change: 2024/02/04: Gesslar
//
// 2024/02/04: Gesslar - Created

private nosave int create_time = time();
private nosave object cloned_by = previous_object();
private nosave string cloned_by_string = file_name(previous_object());

int query_object_create_time() { return create_time; }
int query_object_age() { return time() - create_time; }
object query_cloned_by() { return cloned_by; }
string query_cloned_by_string() { return cloned_by_string; }
