# compile the C# files and then run a test

# all the C# source files must appear in the next line
mcs /debug+ /debug:full /optimize- /out:smc.exe main.cs scanner.cs parser.cs bytecode.cs symboltable.cs interpreter.cs

echo "Compile done, press any key to continue."
read -n1

mono smc.exe -test -dump

echo "Tests done."