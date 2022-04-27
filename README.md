## What I Have Implemented
***
I feel pretty confident in being able to implement things through chapter 9. What this specifically entails is:
#### Expression Level
[x] basic unary operators such as - and !
[x] basic binary arithmetic operations
  [x] \+ \- \* / \ % ^
  * add, subtract, multiply, divide, inverse divide, modulus, exponentiate
  [x] modulus if values can be considered integers is like regulas c modulus0
  [x] modulus on numbers that are not wholes: subtracts right operand from left operand until the left operand is less than the right operand. only works with numbers
[x] logic operations **||** and **&&** - short circuiting
[ ] implied *multiplication* such as $y = 3x + b$
    * I allow for general implied multiplication. when Julia throws errors that you cannot "call" a float, I would instead treat it as multiplication.
    * Example of things that work:
        * 3y
        * 2/(4)(3)  # 0.16
        * 2/((4)(3))# 0.16
        * (2/(4))(3)# julia throws error here due to the Float type, I just multiply since I'm not doing types. this is a Feature I left in the design : D
        * (y)2;     # i also choose to allow this
        * (y)2x     # and this
    * Follow Julia's example however when I disallow:
        * y(3) # cannot call this (in both implementations)
        * y3 # well, this is just a variable, guys
        * 3 y # no spaces!
[ ] variables
  [ ] *identifiers* can begin with "a-z", "A-Z", or ascii symbols such as "_" if that character is not otherwise used by the language (we're not using "#^-+=!@~" and such)
  [ ] I won't allow access to purely underscore variables
  [ ] local variables can be declared using the **local** keyword or without it
  [ ] variables can also be declared in the global scope using **global** anywhere a variable could regularly be declared
  [ ] implicit variable declaration supported
  [ ] *scoping*: I plan to provide functionality of a global scope (with the lexical scoping that Julia is supposed to have) and soft local scopes
[ ] keywords: below is a list of julia's keywords. to keep in the spirit of the language, all of the keywords will be scanned, but while parsing, any keyword that is not supported will be reported as an error.
  [ ] **supported**: continue do else elseif end false for global if local true while
  [ ] **not supported**: baremodule begin break catch export finally try using import let macro module quote struct
[ ] function calling
  [ ] I plan to support passing parameters and calling on identifiers that are pre-defined by the language
  [ ] this includes **println()** or **round()** or other functions I have specified to support

#### Statement Level
[ ] ending
  [ ] \n will matter. newlines can end statements
  [ ] semicolons can end a statement too
    [ ] indentation does not matter
[ ] control flow: **if**
  [ ] I will support julia's if-else statements
  [ ] I also plan to support the intermediate *elseif* statement(s)
  [ ] conditions should not require parentheses around them
[ ] loops
  [ ] I plan to support while loops
  [ ] while loops should have correct local soft scoping
  [ ] you should be able to use break;
  [ ] you should be able to use continue;
[ ] I will support assignment
 

#### Strings
[ ] string escape characters
[ ] string length built-in function
[ ] string concatenation done with *
[ ] multi-line strings with """
[ ] string comparison with ==, !=, <, > etc.

#### Functions
\# as built-in functions:
[ ] print(...) # prints it's input as a string to the console
[ ] println(...) # println gives a new line after it prints
[ ] readline() # this is meant to be CLI input. it takes a line of user input and returns it as a string (including the newline)  

\# we aren't focusing on built-in math functions, but I'd like to implement these:
[ ] round(number) # rounds the number to an integer (but is still of type double internally)
[ ] floor(number) # basically truncates a double 
[ ] ceil(number) # rounds up
[ ] abs(number) # returns the absolute value of the number that's passed in
[ ] sqrt(number) # returns the square root of the number that's passed in


### Specifically Excluding
* non-ascii characters for identifiers && operators (this sounds difficult to do in a language like C++. I've only ever used ASCII characters in it.)
* Infinity (Inf). Division by Zero will probably throw errors. I may change this to the correct behaviour, but probably not. (I'ts just another unnecessary feature.)
* data type assertion - aka. optional static typing (I chose Julia partly because it was *dynamically* typed. I'm not ruining that right now.)
* for loop loops through range(s)
* file IO (this is probably too complicated)
* macros (i'm not gonna build a preprocessor!)
* @printf (no macros)
* fancy structures such as:
  * structs (don't quite know how to yet)
  * abstract types
  * arrays (way to complex in Julia!)
  * array functions such as findfirst() (no arrays)
  * tuples (like arrays)
  * dictionaries (more complicated than arrays)
  * sets (more complicated than lists)
* hard local scoping
* bitwise
* integer division

#### Stretch Goals
[ ] Julia has bitwise logical operators such as &, |, and ~. I'd like to try to implement these.
[ ] relevant updating operators (+=, -=, *=, etc.)
[ ] *symbols*: I'd at least like to be able to treat the ':' symbol operator as an easy way to make a single-word string in the code. so *println(:Ac)* would output *Ac*. This point is not a promise.
[ ] keywords: const function return 
[ ] in julia, you can write statements to execute right after the condition of an **if** or **elseif** without any delimiter. I will put this functionality as a stretch goal cause I'm really not sure how to do it.
[ ] user-defined functions. if I do provide these, it will likely be only the basic function declaration format. no anonymous or single-expression functions.
[ ] string indexing[1:4] or [end]
[ ] string access starts @ index 1
[ ] const variables

### My Resources for This Project
* official docs: https://docs.julialang.org/en/v1/
* Julia StackOverflow: https://stackoverflow.com/questions/tagged/julia - There are great resources and explanations to various questions in Julia such as "What is a symbol?"
* This webpage that serves as a Julia Reference page: https://juliadocs.github.io/Julia-Cheat-Sheet/
* this YouTube tutorial looks great: https://www.youtube.com/watch?v=sE67bP2PnOo
* VS Code and the Julia.exe to try all sorts of Julia programs out in.
