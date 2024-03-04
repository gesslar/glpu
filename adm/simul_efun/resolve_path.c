#include "/adm/obj/simul_efun.h"

/* resolve_path.c

 Kenon @ LPUniversity
 06-Oct-06
 Resolve path simul-efun

*/

/* Last updated by Tacitus on October 6th, 2006 */

string resolve_path(string Current, string Next)
{
   int index;
   int temp;
   string* path_segments;

   if(Next=="here")
      return file_name(environment(this_player())) + ".c";

   if(Next[0]=='~')
   {
      if((Next[1]=='/')||(sizeof(Next)==1))
         Next=user_path(this_player()->query_proper_name())+Next[1..<1];
      else
      {
         index=strsrch(Next, "/");
         if(index==-1) return user_path(Next[1..<0]);
         else Next=user_path(Next[1..index-1])+Next[index..<0];
      }
   }

   if(Next[0]!='/')  Next=Current+"/"+Next;
   path_segments = explode(Next,"/");

   for (index=0;index<sizeof(path_segments);index++)
   {
      if(path_segments[index] == ".") path_segments[index] = "";

      if(path_segments[index] == "..")
      {
         path_segments[index] = "";
         temp = index-1;

         while((temp>=0)&&(!sizeof(path_segments[temp])))
            temp--;

         if(temp!=-1) path_segments[temp]="";
      }
   }

   if(Next[<1]=='/') Next="/"+implode(path_segments, "/")+"/";
   else Next= "/" + implode(path_segments, "/");

   while(strsrch(Next,"//")!=-1) Next = replace_string(Next,"//","/");

   return Next;
}
