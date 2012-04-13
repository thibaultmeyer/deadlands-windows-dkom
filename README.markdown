# DEADLANDS

## What is it?
Deadlands project aim to create a kernel module using DKOM to hides process. It prevents taskmngr from viewing a running process (for example, cmd.exe).


## How to compil?
In order to compile this project, WDK (Windows Driver Kit) must be installed on your computer.

1. Open CMD with the right build environments
2. Go to the src directory
3. Use the command build to compile


## How to use?
Use DeadlandsIOCTL.exe to indicate witch PID/ProcessName you want to set hidden.
>DeadlandsIOCTL.exe name Explorer.exe
>DeadlandsIOCTL.exe pid 2344
