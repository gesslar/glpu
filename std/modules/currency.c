/**
 * @file /std/modules/currency.c
 * @description Currency module that can be inherited for money handling
 *
 * @created 2024/08/01 - Gesslar
 * @last_modified 2024/08/01 - Gesslar
 *
 * @history
 * 2024/08/01 - Gesslar - Created
 */

#include <daemons.h>

// Function prototypes
private mixed check_funds(object tp, string currency, int amount);
private mixed transfer_funds(object from, object to, string currency, int amount);
private mixed check_capacity(object tp, string currency, int amount);
private mixed complex_transaction(object tp, int cost, string currency) ;
private mixed *format_return_currency(mapping m) ;

// Main transaction function
varargs mixed handle_transaction(object tp, int cost, string currency) {
    if (!tp || !objectp(tp)) {
        return "Invalid player object.";
    }

    if (!currency) {
        currency = CURRENCY_D->lowest_currency();
    }

    return complex_transaction(tp, cost, currency);
}

mixed complex_transaction(object tp, int cost, string currency) {
    mapping wealth = tp->query_all_wealth();
    string *currencies = reverse_array(CURRENCY_D->currency_list());
    int total_wealth = tp->query_total_wealth();
    int remaining_cost = cost;
    mapping to_subtract = ([]);
    mapping change = ([]);
    int coin_difference;
    int change_amount;
    int available;
    int to_use;
    int converted_cost;
    int converted_change;

    { // Sanity checks
        // Check for valid currency
        if (member_array(currency, currencies) == -1) {
            return "Invalid currency type.";
        }

        // Check for negative cost
        if (cost < 0) {
            return "Transaction amount must be positive.";
        }
    }

    // Step 1: Check if they can afford it at all
    if (total_wealth < CURRENCY_D->convert_currency(cost, currency, currencies[<1])) {
        return "You don't have enough wealth for this transaction.";
    }

    // Step 2: Try to use the exact currency first
    available = wealth[currency];
    if (available > 0) {
        to_use = min(({available, cost}));
        to_subtract[currency] = to_use;
        remaining_cost -= to_use;
    }

    // Step 3: If we still have remaining cost, iterate from the transaction currency down
    if (remaining_cost > 0) {
        int start_index = member_array(currency, currencies);
        for (int i = start_index; i < sizeof(currencies); i++) {
            string curr = currencies[i];
            available = wealth[curr];
            converted_cost = CURRENCY_D->convert_currency(remaining_cost, currency, curr);

            if (available > 0) {
                to_use = min(({available, converted_cost}));
                to_subtract[curr] = (to_subtract[curr] || 0) + to_use;
                remaining_cost -= CURRENCY_D->convert_currency(to_use, curr, currency);
            }

            if (remaining_cost <= 0) break;
        }
    }

    // Step 4: If we still have remaining cost, iterate from highest to lowest
    if (remaining_cost > 0) {
        foreach (string curr in currencies) {
            if (curr == currency) continue;  // Skip the currency we've already processed
            available = wealth[curr];
            converted_cost = CURRENCY_D->convert_currency(remaining_cost, currency, curr);

            if (available > 0) {
                to_use = min(({available, converted_cost}));
                to_subtract[curr] = (to_subtract[curr] || 0) + to_use;
                remaining_cost -= CURRENCY_D->convert_currency(to_use, curr, currency);
            }

            if (remaining_cost <= 0) break;
        }
    }

    if (remaining_cost > 0) {
        return "You don't have the right combination of coins for this transaction.";
    }

    // Step 5: Calculate change
    if (remaining_cost < 0) {
        change_amount = -remaining_cost;

        foreach (string curr in currencies) {
            converted_change = CURRENCY_D->convert_currency(change_amount, currency, curr);
            if (converted_change > 0) {
                change[curr] = converted_change;
                change_amount -= CURRENCY_D->convert_currency(converted_change, curr, currency);
            }
            if (change_amount <= 0) break;
        }
    }

    // Step 6: Optimize the transaction (convert higher denominations if possible)
    foreach (string curr in currencies) {
        if (to_subtract[curr] && change[curr]) {
            int min_value = min(({to_subtract[curr], change[curr]}));
            to_subtract[curr] -= min_value;
            change[curr] -= min_value;
            if (to_subtract[curr] == 0) map_delete(to_subtract, curr);
            if (change[curr] == 0) map_delete(change, curr);
        }
    }

    // Step 7: Apply the transaction
    foreach (string curr, int amount in to_subtract) {
        tp->add_wealth(curr, -amount);
    }
    foreach (string curr, int amount in change) {
        tp->add_wealth(curr, amount);
    }

    return ({ format_return_currency(to_subtract), format_return_currency(change) });
}

private mixed *format_return_currency(mapping m) {
    mixed *result = ({});
    string *currencies = reverse_array(CURRENCY_D->currency_list());

    foreach(string currency in currencies) {
        if(m[currency] && m[currency] > 0) {
            result += ({ ({ currency, m[currency] }) });
        }
    }

    return result;
}

mixed reverse_transaction(object tp, mixed transaction_result) {
    mixed *subtracted;
    mixed *change;
    mapping to_add;
    mapping to_subtract;
    string currency;
    int amount;

    if (!arrayp(transaction_result) || sizeof(transaction_result) != 2) {
        return "Invalid transaction result";
    }

    subtracted = transaction_result[0];
    change = transaction_result[1];
    to_add = ([]);
    to_subtract = ([]);

    // First, subtract the change
    foreach (mixed *currency_info in change) {
        currency = currency_info[0];
        amount = currency_info[1];
        if (tp->query_wealth(currency) < amount) {
            return "Not enough " + currency + " to reverse the transaction";
        }
        tp->add_wealth(currency, -amount);
        to_subtract[currency] = (to_subtract[currency] || 0) + amount;
    }

    // Then, add back the subtracted coins
    foreach (mixed *currency_info in subtracted) {
        currency = currency_info[0];
        amount = currency_info[1];
        tp->add_wealth(currency, amount);
        to_add[currency] = (to_add[currency] || 0) + amount;
    }

    // Return the net change
    return ({ format_return_currency(to_add), format_return_currency(to_subtract) });
}

string format_return_currency_string(mixed *currency_array) {
    string result = "";

    if (!sizeof(currency_array)) {
        return "None";
    }

    foreach (mixed *currency_info in currency_array) {
        string currency = currency_info[0];
        int amount = currency_info[1];

        if (result != "") {
            result += ", ";
        }

        result += amount + " " + currency;
    }

    return result;
}
// Check if the player has enough funds
private mixed check_funds(object tp, string currency, int amount) {
    if (tp->query_wealth(currency) < amount) {
        return "You don't have enough " + currency + " for this transaction.";
    }
    return 1;
}

// Check if the player can handle the change in capacity
private mixed check_capacity(object tp, string currency, int amount) {
    int current_capacity = tp->query_capacity();
    int coin_difference = amount - tp->query_wealth(currency);

    if (current_capacity < coin_difference) {
        return "You can't carry that much currency.";
    }
    return 1;
}

// Transfer funds from one object to another
private mixed transfer_funds(object from, object to, string currency, int amount) {
    int from_result, to_result;

    from_result = from->add_wealth(currency, -amount);
    if (from_result < 0) {
        return "Failed to remove funds from the source.";
    }

    to_result = to->add_wealth(currency, amount);
    if (to_result < 0) {
        // Revert the transaction if adding to the destination fails
        from->add_wealth(currency, amount);
        return "Failed to add funds to the destination.";
    }

    return 1;
}

// Convert currency for a transaction
mixed convert_for_transaction(object tp, int cost, string from_currency, string to_currency) {
    int converted_amount;
    mixed result;

    if (!CURRENCY_D->valid_currency_type(from_currency) ||
        !CURRENCY_D->valid_currency_type(to_currency)) {
        return "Invalid currency type.";
    }

    converted_amount = CURRENCY_D->convert_currency(cost, from_currency, to_currency);

    result = check_funds(tp, from_currency, cost);
    if (stringp(result)) return result;

    result = check_capacity(tp, to_currency, converted_amount);
    if (stringp(result)) return result;

    if (tp->convert_wealth(from_currency, to_currency, cost)) {
        return converted_amount;
    }

    return "Currency conversion failed.";
}

// Function to check if an object can afford a transaction
mixed can_afford(object ob, int cost, string currency) {
    if (!ob || !objectp(ob)) {
        return "Invalid object.";
    }

    if (ob->query_wealth(currency) >= cost) {
        return 1;
    }

    return "Not enough funds.";
}

// Function to format currency for display
string format_currency(int amount, string currency) {
    return amount + " " + currency;
}
