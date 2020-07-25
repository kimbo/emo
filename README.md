# emo :joy: The Emotional Language :sob:

![Build](https://github.com/kimbo/emo/workflows/Build,%20Test,%20and%20Coverage/badge.svg)
[![codecov](https://codecov.io/gh/kimbo/emo/branch/master/graph/badge.svg)](https://codecov.io/gh/kimbo/emo)

In school, I took a class where we wrote a lexer and a parser for [datalog](https://en.wikipedia.org/wiki/Datalog) to do relational algebra type stuff.
I learned a lot, but it wasn't that exciting.
If I was going to write something that could interpret and execute a language, I wanted it to be my own language, and I wanted it to be fun.

So I decided to try and write my own programming language called `emo`.
I'm not super set on the design yet, but I'm trying to keep it simple.
The main idea is to have a simple language with "enhance emoji support", meaning that emojis are built in to the standard library.
I realize I could just build a library in another language that would do something like this, but I wouldn't learn nearly as much.
Besides, who doesn't want to write `emo` code?

## emo design

- :+1: All "key-words" are shortened to three letter variants, except for `if`.
    - `fun`ction, `whi`le, `num`ber, `ret`urn, `str`ing
- :+1: Emojis are of the Github variant
- :+1: Emojis can be functions or used as a `str`ing.
- :-1: Floating point numbers are not supported
- :-1: A lot of other things are not supported

## Roadmap

- [x] lexer
- [ ] parser
  - [ ] emoji library
- [ ] compiled or interpreted? I'm leaning towards compiled.
