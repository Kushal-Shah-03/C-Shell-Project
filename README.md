[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/76mHqLr5)
# Description

Submission for Part A for mid deadline

running make creates an a.out file, on running a.out with sudo access, you can run part A
sudo access is needed for some parts of proclore

# Assumptions

1. Since it is given in doubts document that the shell will be run in root, while checking for permissions of file or folder in seek command -e flag only checking for user perms

2. If in pastevents execute index if the index value is greater than the number of lines currently stored in pastevents then that command isn't stored in the pastevents list and there is no output for the command, and if the index value is in the range then it is replaced by the respective command and thus stored in pastevents. exit case of the terminal has not been handled for pastevents

3. The string that is being stored in pastevents is after being tokenized, so that we can compare the two tokenized strings to avoid 
sleep 3 ; sleep 2 and sleep 3; sleep 2 as different commands in pastevents

4. While rounding time to print in the prompt for foreground process, I have rounded down time to nearest integer

To calculate the time taken by the foreground process while printing in the prompt, I consider the last foreground process with time taken >= 2 and only print the command for that process,
for eg sleep 3 ; sleep 2 ; sleep 1,
2 will be printed as 2 is the last running process in the command which took time >=2

5. If there is an invalid command like pastevents <some string> or pastevents purge <some random string> it get's stored in the pastevents and doesn't perform pastevents or pastevents purge command respectively.

6. In peek the direcotries and files are sorted in lexicographic order i.e according to ASCII values
 
7. If I start a background process then kill the terminal and start the terminal again, if the background process get's terminated after the new terminal has started, the outputs of the process won't be shown on this terminal nor will the terminal show that the process ended, this has been handled similar to how it done in shell

8. Any process that is not started by my terminal, is considered a background processes, and any process started by my terminal in background is also considered a background process while indicating +/- all other processes are considered foreground.

eg. if i do proclore 1 (since my terminal hasn't started it, it's considered background)
eg. if i do proclore (with no arguments), it'll show R+ since it's foreground
eg. if i do sleep 5 & and proclore the pid, it'll be considered background

9. proclore command gives absolute path for the executable path

10. seek command checks the files/directories for the existence of the given input as a prefix of their names  

Final commit

11. In piping and background like sleep 20 | sleep 10 & the command after the last pipe's time is considered to print if the process terminated succesfully, so it will print process terminated succesffuly after 10s and not 20

12. If we do fg for a pid and then stop it using ctrl Z then the command stored in activities is fg <pid> with the same pid

13. Neonate command since it's in raw mode can only be stopped by pressing the given key 'x' without caps, it will not get modified by ctr c ctr d

14. only vim doesn't work properly while running in background and while running it using fg

15. iMan only prints Name Synopsis Description, the parsing is very basic, if command is not found prints No command found

16. Not handling quotes ('' and ""), letting execvp handle it.

17. Since I have color coded some error messages they aren't printed using perror commands

18. For float values in neonate, the command rounds it off to lower integer and executes

19. Only execvp commands are run as background no user defined command can be run as background

20. seek recursively checks in hidden files as well.