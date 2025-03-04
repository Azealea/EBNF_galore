# EBNF_Galore
### Maker of random script with EBNF based structure

I have added some specific things to help writing the base structure, but it
does those EBNF operation :

 [ expr ]           => expr will appear 0 or 1 time
 { expr }           => expr will appear 0 or inf time
 ( expr )           => grouping
 expr_A | expr_B    => it will chose between expr_A and expr_B at random


The rules are declared as per usual :

 "identifier = expr ;"

 ( identifier follow this regex : [a-z][a-zA-Z0-9_]* )


The rules that will taken as root must be preceded by a explamation mark :

 "!foo = "bar" ;"


To help random not exploding when confronted with choices, an number can be inserted before a choice to specify the weight,
if not specified it will be defaulted to 1 :

 " 2 expr_A | expr_B "

 ( 2 third chance of taking expr_A, 1 third chance of taking expr_B)


### TODO
- Comments as (* this is a comment \*\)
- Limit number of repetition


