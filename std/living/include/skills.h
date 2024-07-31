#ifndef __SKILLS_H__
#define __SKILLS_H__

void init_skills();
void wipe_skills();
varargs void initialize_missing_skills(mapping skill_set, string curr_path);
varargs int add_skill(string skill, float level);
int remove_skill(string skill);
float query_skill(string skill);
varargs int query_skill_level(string skill, int raw);
int set_skill_level(string skill, float level);
mapping query_skills();
void set_skills(mapping s);
int train_skill(string skill, float progress);
int query_skill_progress(string skill);
int modify_skill_level(string skill, int level);

#endif // __SKILLS_H__
