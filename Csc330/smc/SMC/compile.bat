:: compile the C# files and then run a test

:: all the C# source files must appear in the next line
csc /debug+ /debug:full /optimize- /out:smc.exe main.cs scanner.cs parser.cs bytecode.cs symboltable.cs interpreter.cs

pause

smc -test -dump

pause
