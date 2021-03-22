# Features
- batch implementation (can run commands given in a file)
- asyncronous commands(more than 2 asyncronous commands allowed)
- can run processes in background
- is able to handle errors 
- can pipeline to run multiple processes example: ```pwd | cd .. | pwd ```
- supports some internal commands such as ```cd```, ```ls```, ```pwd```, though it does not support all of them

# Execution
- For Shell mode ```gcc shell.c && ./a.out```

- For batch mode ```gcc shell.c && ./a.out cmds.txt```, where ```cmds.txt``` contain the commands to run.

Note: For pipelines to work properly, commands should be space seperated.
For example: ```pwd | cd .. | pwd ```. There should be space before and after "|"
