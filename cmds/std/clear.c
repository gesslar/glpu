/* clear.c
Icoz @ LPUniversity
21-MAY-06
Standard command

*/

inherit CMD ;

int main(object caller, object room, string str)
{
    write("%^CLEAR%^\n");
    return 1;
}

string help(object caller)
{
    return(HIW + "SYNTAX: " + NOR + "clear\n\n"
    "This command clears the screen\n");
}
