# EBNF_Galore
## Maker of random script with EBNF based structure

This program will take in input a file that describe a language with EBNF and make
a derivation out of it.

## Details

I have added some specific syntax to help writing the base structure, but it
does follow these EBNF operation :

 - [ expr ]           => expr will appear 0 or 1 time
 - { expr }           => expr will appear 0 to inf time
 - ( expr )           => grouping
 - expr_A | expr_B    => it will chose between expr_A and expr_B at random

The rules are declared as per usual :

 "identifier = expr ;"

 ( identifier follow this regex : [a-z][a-zA-Z0-9_]* )

## Syntax Extensions

Here is specified the syntax addition to EBNF to helkp control the output

The rules that will taken as root must be preceded by a explamation mark :

 "!foo = "bar" ;"

### Random control

#### Choices

To help random not exploding when confronted with choices, an number can be inserted before a choice to specify the weight,
if not specified it will be defaulted to 1 :

 " 2 expr_A | expr_B "

 ( 2 third chance of taking expr_A, 1 third chance of taking expr_B)

#### Repetition

To specify the maximum number of repetition :

" { 5 expr } "    => 5 repetition maximum

#### Optional

To specify the chance of taking a optional expression :

" [ 3 1 expr ] "   =>  3 chance in 4 of taking the expr, or else discard it

## TODO
- Comments as (* this is a comment \*\)


