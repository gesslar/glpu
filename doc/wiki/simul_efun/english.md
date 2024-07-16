## cap_words

## SYNOPSIS

    string cap_words(string str)

### PARAMETERS

    string str - The string to capitalize.

### RETURNS

    string - The capitalized string.

## DESCRIPTION

Capitalizes the first letter of each word in a string.

## subjective

## SYNOPSIS

    string subjective(mixed ob)

### PARAMETERS

    mixed ob - The object or gender string to convert.

### RETURNS

    string - The subjective pronoun.

## DESCRIPTION

Returns the subjective pronoun corresponding to the object's
gender. Defaults to "it" for non-string or unknown gender.

## possessive_noun

## SYNOPSIS

    string possessive_noun(mixed ob)

### PARAMETERS

    mixed ob - The object or string to convert to possessive form.

### RETURNS

    string - The possessive form of the noun.

## DESCRIPTION

Returns the possessive form of a noun. If the noun ends with 's',
it adds an apostrophe; otherwise, it adds 's.

## cap_significant_words

## SYNOPSIS

    varargs string cap_significant_words(string str, int title)

### PARAMETERS

    string str - The string to capitalize.
    int [title=0] - Whether to capitalize the first word as a title.

### RETURNS

    string - The string with significant words capitalized.

## DESCRIPTION

Capitalizes significant words in a string, ignoring certain
insignificant words. Optionally capitalizes the first word
as a title.

## possessive

## SYNOPSIS

    string possessive(mixed ob)

### PARAMETERS

    mixed ob - The object or gender string to convert.

### RETURNS

    string - The possessive adjective.

## DESCRIPTION

Returns the possessive adjective corresponding to the object's
gender. Defaults to "its" for non-string or unknown gender.

## possessive_pronoun

## SYNOPSIS

    string possessive_pronoun(mixed ob)

### PARAMETERS

    mixed ob - The object or gender string to convert.

### RETURNS

    string - The possessive pronoun.

## DESCRIPTION

Returns the possessive pronoun corresponding to the object's
gender. Defaults to "its" for non-string or unknown gender.

## objective

## SYNOPSIS

    string objective(mixed ob)

### PARAMETERS

    mixed ob - The object or gender string to convert.

### RETURNS

    string - The objective pronoun.

## DESCRIPTION

Returns the objective pronoun corresponding to the object's
gender. Defaults to "it" for non-string or unknown gender.

## reflexive

## SYNOPSIS

    string reflexive(mixed ob)

### PARAMETERS

    mixed ob - The object or gender string to convert.

### RETURNS

    string - The reflexive pronoun.

## DESCRIPTION

Returns the reflexive pronoun corresponding to the object's
gender. Defaults to "itself" for non-string or unknown gender.

