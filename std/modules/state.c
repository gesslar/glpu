#include <classes.h>

inherit CLASS_STATE;

void add_step(string step, function on_enter, function on_exit);
void set_initial_step(mixed step);
private void set_current_step(mixed *step);
void complete_step(string step);
void start_step(mixed *step);
mixed *get_next_step(string step);
string query_step();
public mixed *find_step(mixed step);

private nosave class StateStep *state_steps = ({});
private nosave int current_step_index;
private nosave class StateStep current_step;

/**
 * Add a new step to the end of the state machine.
 *
 * @param {string} name - The name or label of the step.
 * @param {function} on_enter - The enter trigger of the step.
 * @param {function} on_exit - The exit trigger of the step.
 */
void add_step(string name, function on_enter, function on_exit) {
  class StateStep state_step;

  state_step = new(class StateStep,
    name: name,
    enter: on_enter,
    exit: on_exit,
    completed: 0
  );

  push(ref state_steps, state_step);
}

/**
 * Sets the initial step of the state machine. Can be an index
 * or the step name.
 *
 * @param {string|int} step - The index of the step or the step's name.
 */
void set_initial_step(mixed step) {
  mixed *found = find_step(step);

  if(!found)
    error("No such step.");

  set_current_step(found);
}

private void set_current_step(mixed *step) {
  current_step_index = step[0];
  current_step = step[1];
}

void complete_step(string step) {
  mixed *next_step;

  if(current_step.name != step || current_step.completed)
    return;

  if(valid_function(current_step.exit))
    catch((*current_step.exit)());

  current_step.completed = 1;

  next_step = get_next_step(step);

  if(next_step)
    start_step(next_step);
}

void start_step(mixed *step) {
  if(!step)
    return;

  if(step[1].completed)
    return;

  set_current_step(step);

  if(valid_function(current_step.enter))
    catch((*current_step.enter)());
}

mixed *get_next_step(string step) {
  mixed *found, *next;

  found = find_step(step);

  if(!found)
    return 0;

  return find_step(found[0]+1);
}

class StateStep query_step() {
  return current_step;
}

/**
 * Finds and returns a step from the available state steps based on either
 * its name or its index.
 *
 * @param {string|int} step - The step to find by its name or index.
 * @returns { ({int,class StateStep}) } A tuple representing the step's index and the step.
 */
public mixed *find_step(mixed step) {
  mixed *result = allocate(2);

  assert_arg(
    (stringp(step) && strlen(step)) || (!nullp(step) && intp(step)),
    1,
    "Invalid step."
  );

  if(intp(step)) {
    if(in_range(step, state_steps)) {
      result[0] = step;
      result[1] = state_steps[step];
    } else {
      return null;
    }
  } else {
    int index;

    if((index = find_index(state_steps, (: $1.name == $(step) :))) == -1) {
      return null;
    } else {
      result[0] = index;
      result[1] = state_steps[index];
    }
  }

  return result;
}
