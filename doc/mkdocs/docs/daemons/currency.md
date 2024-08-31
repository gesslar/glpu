`CURRENCY_D` is essential for managing currency in the game. It provides
functions to provide information about currency and to convert between them.

## Configuration

The currency system is configured in the `default.json` and `config.json` files.
The configuration is an array of mappings, where each mapping represents a
currency and its value. The first currency in the array is the base currency.

!!! info "adm/etc/default.json"

    ```json
    "CURRENCY" : [
        [ "copper", 1 ],
        [ "silver", 10 ],
        [ "gold", 100 ],
        [ "platinum", 1000 ]
    ]
    ```

The currency daemon retrieves the currency configuration from the
[`CONFIG_D`](config.md) daemon.

!!! info

    If you update the `CURRENCY` array in the `default.json` or `config.json`
    files, you should run the `master` command to update the game's currency
    system.

## API

### `convert_currency`

Converts an amount of currency from one type to another.

`#!C int convert_currency(int amount, string from_currency, string to_currency)`

* `amount (int)` - The amount of currency to convert.
* `from_currency (string)` - The currency to convert from.
* `to_currency (string)` - The currency to convert to.

!!! example

    ```c
    int converted_amount = convert_currency(100, "gold", "platinum");
    ```

### `fconvert_currency`

Converts an amount of currency from one type to another. Similar to `convert_currency`, but returns a float, enabling fractional currency values.

`#!C float fconvert_currency(int amount, string from_currency, string to_currency)`

* `amount (int)` - The amount of currency to convert.
* `from_currency (string)` - The currency to convert from.
* `to_currency (string)` - The currency to convert to.

!!! example

    ```c
    float converted_amount = fconvert_currency(100, "gold", "platinum");
    ```

### `lowest_currency`

Returns the lowest currency in the currency system.

`#!C string lowest_currency()`

### `highest_currency`

Returns the highest currency in the currency system.

`#!C string highest_currency()`

### `currency_list`

Returns a list of all currencies in the currency system.

`#!C string *currency_list()`

### `currency_value`

Returns the value of a currency in the currency system.

`#!C int currency_value(string currency)`

### `get_currency_map`

Returns the currency map containing all currencies and their values.

`#!C mapping get_currency_map()`
