# pipex

**pipex** is a simple implementation of the `|` pipe redirection in Unix-like operating systems, written in C. This project is inspired by the `pipe()` system call and aims to provide a basic understanding of how processes communicate and how shell commands involving pipes are executed internally.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Example](#example)
- [Multiple Commands and Heredoc](#multiple-commands-and-heredoc)
- [Useful Links](#useful-links)

## Overview

In Unix-like operating systems, the `|` operator allows the output of one command to be passed as input to another command. This project aims to replicate this functionality using C programming language and system calls.

## Features

- **Basic Pipe Redirection**: Redirects the output of one command as input to another command.
- **Error Handling**: Provides basic error handling for file operations and system calls.
- **Support for Multiple Commands**: Allows chaining of multiple commands together using pipes.

## Getting Started

To get started with **pipex**, follow these steps:

1. **Clone the Repository**: Clone this repository to your local machine using<br> `git clone https://github.com/theVeryPulse/pipex.git`.

2. **Compile the Code**: Navigate to the repository directory and compile the code using `make`.

3. **Run pipex**: Execute the program with appropriate arguments. For example:<br>
```./pipex file1 cmd1 cmd2 file2```


## Usage

The general syntax for using **pipex** is as follows:

In `./pipex file1 cmd1 cmd2 file2` :


- `file1`: Input file whose contents will be redirected to the first command.
- `cmd1`: First command to be executed.
- `cmd2`: Second command to be executed.
- `file2`: Output file where the final result will be stored.

## Example

Here's an example demonstrating how to use **pipex**:

`./pipex infile "grep foo" "grep bar" outfile`


This command will execute `grep foo` on `infile` and then filter the output using `grep bar`. The final result will be stored in `outfile`.

## Multiple Commands and Heredoc

Run `make bonus` to compile pipex supporting multiple commands and heredoc.

- Syntax to run multiple commands:<br>
`./pipex file1 cmd1 cmd2 ... cmdn file2`<br>
acts as if<br>
`< infile cmd1 | cmd2 | ... | cmdn > outfile`

- Heredoc and append:<br>
`./pipex heredoc END cmd1 cmd2 ... cmdn outfile`<br>
acts as if<br>
`<< END cmd1 | cmd2 | ... | cmdn >> outfile`

## Useful Links
[Unix Processes in C | YouTube](https://www.youtube.com/watch?v=cex9XrZCU14&list=PLfqABt5AS4FkW5mOn2Tn9ZZLLDwA3kZUY&pp=iAQB)

