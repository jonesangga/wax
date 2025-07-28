# wax

**wax** is a tiny language designed to transpile to C.

### [Playground](https://waxc.netlify.app/) | [Quickstart](./QUICKSTART.md) | [Examples](./examples) | [IDE](https://github.com/LingDong-/wax4vscode)

The goal of wax is to be a "common subset" of most major imperative programming languages. By lacking the unique fancy features in each of these languages and being as boring as possible, wax transpiles to all of them seamlessly, producing outputs that are:

- Readable: The output code looks just like the input code.
- Editable: A human programmer should be able to work from the output even if the original wax source is unavailable.
- Integrable: The output code can be imported as libraries to use with the rest of the target language (in addition to just being runnable alone).

These of course, from the programmers' perspective, come at the cost of losing some of the interesting features offered by other languages. Nevertheless, wax retains the crucial bits for a programming language to be productive.

The syntax of wax is inspired by WebAssembly Text Format (wat), hence the name. Though it uses S-expressions reminiscent of the Lisp family, it is actually quite **imperative** and most resemblant of C in its design. The idea of transpiling to many languages is inspired by Haxe.


wax is currently experimental, so there might be bugs as well as aspects to be improved, in which case PR and Issues are very much appreciated.


## Hello World

```scheme
(func main (result int)
  (print "hello world!")
  (return 0)
)
```

Newlines and indentations are entirely cosmetic. You can use any type of brackets anywhere (`()` `[]` `{}`). You can mix different types of brackets if you somehow prefer that.

```scheme
[func main [result int] [print "hello world!"] [return 0]]

{func main {result int} {print "hello world!"} {return 0}}

{func main [result int] 
    (print "hello world!") 
    (return 0)
}
```

Here's an in-place quicksort to get a quick taste of the language:

```scheme
;; sort array in-place for index range [lo,hi] inclusive
(func qksort_inplace (param A (arr float)) (param lo int) (param hi int)
    (if (>= lo hi) (then
        (return)
    ))
    (let pivot float (get A lo))
    (let left  int lo)
    (let right int hi)
    (while (<= left right) (do
        (while (< (get A left) pivot) (do
            (set left (+ left 1))
        ))
        (while (> (get A right) pivot) (do
            (set right (- right 1))
        ))
        (if (<= left right) (then
            (let tmp float (get A left))
            (set A left (get A right))
            (set A right tmp)
            (set left  (+ left 1))
            (set right (- right 1))
        ))
    ))
    (call qksort_inplace A lo right)
    (call qksort_inplace A left hi)
)

(func qksort (param A (arr float))
    (if (! (# A)) (then
        (return)
    ))
    (call qksort_inplace A 0 (- (# A) 1))
)
```

As you might have noticed, writing in wax is pretty much like writing an abstract syntax tree directly!

There're many more examples, check them out [here](./examples) or on the [online playground](https://waxc.netlify.app/).


## Overview

- wax is strongly statically typed.
- wax has built-in support for arrays, hashtables and structs.
- wax supports C-like macros, allowing specifying different behavior/branches for each compile target, as well as embedding target code directly.
- syntax is simple: an expression is always a list of tokens enclosed in parenthesis `()`, and the first token is always a keyword/operator. There're 50 keywords in total.
- wax does not support OOP (meaning that you get `struct`s to pack data together, but you cannot blend methods into structs), nor does it support functional programming.
- wax does not have a boolean: zero is false, non-zero is true.
- wax is not garbage-collected. However, it does have constructs to facilitate memory management and make leaky bugs less likely. On compile targets that do support garbage collection (e.g. Java, JS), explicit freeing of resources is not required, and theoretically you can ignore memory management altogether if you intend to compile to these targets only. Check out the [Array/Vector/Struct sections in Quickstart](https://github.com/LingDong-/wax/blob/main/QUICKSTART.md#arrays-and-vectors) for details.

## The Compiler

This repo contains a reference implementation of wax called `waxc`, written from scratch in C99.

- Compiles from wax to C.
- It seems pretty fast. Compiling a 700 lines file takes 0.015 seconds on Macbook Pro 2015. Comparison: the output TypeScript, which is also 700 lines long, took `tsc` 1.5 seconds to compile. 
- Additionally, it can emit a very detailed and low-level syntax tree in JSON format. (If your favourite language is not a supported wax target yet, it's not too hard to go from this file and write a code generator :)
- It can print the tokenization and the abstract syntax tree to terminal.
- Usage:

```
 _____                                           
|||'  |                                          
|''   |                                          
|_WAX_| Compiler                                 

built Oct 27 2020                               

USAGE: waxc [options] code.wax                   

OPTIONS:                                         
--c     path/out.c     transpile to c            
--json  path/out.json  syntax tree to JSON file  
--tokens               print tokenization        
--ast                  print abstract syntax tree
--silent               don't print info          
--help                 print this message 
```

### Example

To compile the `fib.wax` example included in the example folder to C, and print the abstract syntax tree to terminal:

```bash
./waxc examples/fib.wax --c fib.c --ast
```

Now compile the C output with gcc and run the example:

```bash
gcc fib.c
./a.out
```


### Compiling the Compiler

You need:

- A C compiler that supports C99. e.g. `gcc` or `clang`.

To compile:

```bash
gcc src/wax.c -o waxc
```

That's it, no dependencies.

Alternatively you can run the Makefile:

- `make c`. Compile it.
- `make co`. Compile it with `-std=c99 -O3 -std=c99 -pedantic -Wall`.


### Get Started

Now that the compiler is compiled, head over to [QUICKSTART.md](./QUICKSTART.md) for a tour of language features!
