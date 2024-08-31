# Currency

To use the currency system, you need to inherit `M_CURRENCY` in your mob or
object. This will give you access to the API to manage currency transactions.
Refer to the documentation on [`CURRENCY_D`](../../daemons/currency.md) for
more details on how the system operates.

## `handle_transaction`

Processes the transaction, checking if the player can afford it and adjusting
their balance accordingly.

`#!C mixed handle_transaction(object tp, int cost, string currency)`

* `tp (object)` - The player object making the transaction.
* `cost (int)` - The cost of the transaction.
* `currency (string)` - The type of currency to use.

!!! example

    ```c
    mixed result = handle_transaction(player, 100, "gold");
    ```

## `check_funds`

Checks if the player has enough funds in the specified currency for a
transaction.

`#!C mixed check_funds(object tp, string currency, int amount)`

* `tp (object)` - The player object whose funds are being checked.
* `currency (string)` - The type of currency to check.
* `amount (int)` - The amount of currency to check against.

!!! example

    ```c
    mixed result = check_funds(player, "gold", 50);
    ```

## `transfer_funds`

Transfers a specified amount of currency from one object to another.

`#!C mixed transfer_funds(object from, object to, string currency, int amount)`

* `from (object)` - The object from which funds are being transferred.
* `to (object)` - The object to which funds are being transferred.
* `currency (string)` - The type of currency being transferred.
* `amount (int)` - The amount of currency to transfer.

!!! example

    ```c
    mixed result = transfer_funds(player1, player2, "silver", 30);
    ```

## `convert_for_transaction`

Converts a specified amount of currency from one type to another.

`#!C mixed convert_for_transaction(object tp, int cost, string from_currency, string to_currency)`

* `tp (object)` - The player object.
* `cost (int)` - The amount to convert.
* `from_currency (string)` - The currency to convert from.
* `to_currency (string)` - The currency to convert to.

!!! example

    ```c
    mixed result = convert_for_transaction(player, 50, "silver", "gold");
    ```
