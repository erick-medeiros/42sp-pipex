# pipex

This project is about handling pipes

## Final Score

<img height=150px src=https://github.com/erick-medeiros/guide42/blob/main/grade/grade_125_100.png alt="eandre-f score"/>

## The Project

* The program should replicate the behavior of bash when using pipes in commands that have files as input and output

Build mandatory:

```bash
make
```

Build bonus:

```bash
make bonus
```

### Mandatory

* Handle two commands

```bash
# Program:
./pipex file1 cmd1 cmd2 file2

# Bash:
< file1 cmd1 | cmd2 > file2
```

Example:

```bash
./pipex infile "cat" "wc -l" outfile
```

### Bonus

* Handle multiple pipes

```bash
# Program:
./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2

# Bash:
< file1 cmd1 | cmd2 | cmd3 ... | cmdn > file2
```

* Support « and » when the first parameter is "here_doc"

```bash
# Program:
./pipex here_doc LIMITER cmd cmd1 file

# Bash:
cmd << LIMITER | cmd1 >> file
```
