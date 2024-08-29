#include <simul_efun.h>
#include <daemons.h>

/**
 * @simul_efun emit
 * @description Emit a signal to all objects that have registered a slot for
 *              the signal.
 * @param {int} sig - signal number
 * @param {mixed...} arg - arguments to pass to the signal
 */
void emit(int sig, mixed arg...) {
    catch(SIGNAL_D->dispatch_signal(sig, arg...)) ;
}

/**
 * @simul_efun slot
 * @description Register a slot for a signal.
 * @param {int} sig - signal number
 * @param {string} func - function to call when the signal is emitted
 * @returns {int} - `SIG_SLOT_OK` if the slot was registered successfully. See `include/signal.h` for other return values.
 */
int slot(int sig, string func) {
    return SIGNAL_D->register_slot(sig, previous_object(), func) ;
}

/**
 * @simul_efun unslot
 * @description Unregister a slot for a signal.
 * @param {int} sig - signal number
 * @returns {int} - `SIG_SLOT_OK` if the slot was unregistered successfully. See `include/signal.h` for other return values.
 */
int unslot(int sig) {
    return SIGNAL_D->unregister_slot(sig, previous_object()) ;
}

/**
 * @simul_efun signal_d
 * @description Get the signal daemon object.
 * @returns {object} - signal daemon object
 */
object signal_d() {
    return load_object(SIGNAL_D) ;
}
