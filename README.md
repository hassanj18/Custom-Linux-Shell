
```markdown
# My Shell Project 🚀

## Introduction 🌟
This Shell supports command line pipes, FIFOs, as well as input/output and error redirection. Additionally, it offers a command history feature to enhance user experience. 

## Features 🛠️
- **Command Line Pipes**: Use multiple commands connected by `|` to process output efficiently.
- **FIFOs**: Create named pipes for inter-process communication.
- **Input/Output Redirection**: Redirect input and output using `>` and `<`.
- **Command History**: Retain the last 10 commands, allowing you to execute them easily.

### Example Usage 📜
1. Create a FIFO:
   ```bash
   $ mkfifo fifo1
   ```
2. Redirect output to a file:
   ```bash
   $ man ls 1> ls.dat
   ```
3. Use pipes:
   ```bash
   $ cat fifo1 | grep ls | wc -l
   ```
4. Combine sorting with tee:
   ```bash
   $ sort ls.dat | tee fifo1 | wc -l
   ```
5. List command history:
   ```bash
   $ history
   ```
6. Execute last command:
   ```bash
   $ !!
   ```
7. Execute specific command from history:
   ```bash
   $ !4
   ```

## Command History Feature 📚
- **View History**: Enter `history` to see the last 10 commands.
- **Execute Last Command**: Type `!!` to run the most recent command.
- **Execute Specific Command**: Type `!N` where `N` is the command number to execute that command.
  
### Example Output 
For a history consisting of:
- `ps`
- `ls -l`
- `top`
- `cal`
- `who`
- `date`

The command `history` will output:
```
6: ps
5: ls -l
4: top
3: cal
2: who
1: date
```

## Error Handling ⚠️
- If history is empty, entering `!!` displays: `No commands in history.` 
- If you try to execute a command that does not exist in history using `!N`, it displays: `No such command in history.`

## Getting Started 🏁
1. Clone the repository:
   ```bash
   git clone https://github.com/hassanj18/Custom-Linux-Shell
   ```
2. Navigate into the project directory:
   ```bash
   cd Custom-Linux-Shell
   ```
3. Compile the program (if applicable):
   ```bash
   g++ shell.cpp -o shell
   ```
4. Run the shell:
   ```bash
   ./yshell
   ```

## Contributing 🤝
Feel free to contribute! Open issues or submit pull requests for any improvements.


