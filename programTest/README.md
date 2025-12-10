This folder will contain all the "source code" of HoLang.

I will be using the source code in this folder for testing.

# Philosophy

I am currently just following around the [this](https://www.youtube.com/playlist?list=PLUDlas_Zy_qC7c5tCgTMYq2idyyT241qs) because I don't want to reinvent everything from scratch and follow along the tutorial. I am following this tutorial as only a reference and I am not using this as a one to one guide nor am I following the same architecture as the playlist.

# Steps

These are the steps I am following to generate code:
- Tokenization
- Parse Tree Generation
- Code Generation

# Tokenization

Tokenization is the first step of the process where I go through each character and parse it. The source code is converted into tokens of various types like literals, keywords etc.

# Parse Tree Generation

This is where I turn the tokens into tangible pieces of logic.

Tokens are put into a `Expression tree` where each `sub tree is an Expression tree` and each `leaf node is a token`.

# Code generation

This is where I convert the tree to compiled/transpiled code. Honestly this is going to be easy if I make a proper parse tree.