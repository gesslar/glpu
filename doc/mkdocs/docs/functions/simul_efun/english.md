---
title: english
---
# english.c

## cap_words

### Synopsis

```c
string cap_words(string str)
```

### Parameters

* `string str` - The string to capitalize.

### Returns

`string` - The capitalized string.

### Description

Capitalizes the first letter of each word in a string.

## possessive_proper_noun

### Synopsis

```c
string possessive_proper_noun(mixed ob)
```

### Parameters

* `mixed ob` - The object or string to convert to possessive form.

### Returns

`string` - The possessive form of the proper noun.

### Description

Returns the possessive form of a proper noun. Applies 's to the
end of the noun.

## remove_article

### Synopsis

```c
string remove_article(string str)
```

### Parameters

* `string str` - The string to remove the article from.

### Returns

`string` - The string with the article removed.

### Description

Removes an article from a string. If the string begins with
"the ", "a ", or "an ", removes the article.

## add_article

### Synopsis

```c
varargs string add_article(string str, int definite)
```

### Parameters

* `string str` - The string to add the article to.
* `int [definite=0]` - Whether to add the definite article.

### Returns

`string` - The string with the article prepended.

### Description

Adds an article to a string. If definite is true, adds "the";
otherwise, adds "a" or "an" depending on the noun's initial
letter.

## subjective

### Synopsis

```c
string subjective(mixed ob)
```

### Parameters

* `mixed ob` - The object or gender string to convert.

### Returns

`string` - The subjective pronoun.

### Description

Returns the subjective pronoun corresponding to the object's
gender. Defaults to "it" for non-string or unknown gender.

## possessive_noun

### Synopsis

```c
string possessive_noun(mixed ob)
```

### Parameters

* `mixed ob` - The object or string to convert to possessive form.

### Returns

`string` - The possessive form of the noun.

### Description

Returns the possessive form of a noun. If the noun ends with 's',
it adds an apostrophe; otherwise, it adds 's.

## cap_significant_words

### Synopsis

```c
varargs string cap_significant_words(string str, int title)
```

### Parameters

* `string str` - The string to capitalize.
* `int [title=0]` - Whether to capitalize the first word as a title.

### Returns

`string` - The string with significant words capitalized.

### Description

Capitalizes significant words in a string, ignoring certain
insignificant words. Optionally capitalizes the first word
as a title.

## possessive

### Synopsis

```c
string possessive(mixed ob)
```

### Parameters

* `mixed ob` - The object or gender string to convert.

### Returns

`string` - The possessive adjective.

### Description

Returns the possessive adjective corresponding to the object's
gender. Defaults to "its" for non-string or unknown gender.

## possessive_pronoun

### Synopsis

```c
string possessive_pronoun(mixed ob)
```

### Parameters

* `mixed ob` - The object or gender string to convert.

### Returns

`string` - The possessive pronoun.

### Description

Returns the possessive pronoun corresponding to the object's
gender. Defaults to "its" for non-string or unknown gender.

## objective

### Synopsis

```c
string objective(mixed ob)
```

### Parameters

* `mixed ob` - The object or gender string to convert.

### Returns

`string` - The objective pronoun.

### Description

Returns the objective pronoun corresponding to the object's
gender. Defaults to "it" for non-string or unknown gender.

## reflexive

### Synopsis

```c
string reflexive(mixed ob)
```

### Parameters

* `mixed ob` - The object or gender string to convert.

### Returns

`string` - The reflexive pronoun.

### Description

Returns the reflexive pronoun corresponding to the object's
gender. Defaults to "itself" for non-string or unknown gender.

