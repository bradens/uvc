/* 
 * $ Header: ssi.c ssi.h
 * $ Author: Braden Simpson
 * $ Student Number: V00685500
 * $ Date : September 20/2010
 * 
 * =========================================================================
 * 
 * Simple Shell Interpreter
 * Programming Assignment 1
 * Csc 360
 * 
 * Bonus feature : ls -R (I realize that I asked the professor too late 
 * for this bonus feature, but I included the code anyways.)
 *
 * In main the program sits in a while loop, calling readline() and then
 * once the user passes in a line terminated by a newline character, then
 * ParseToken() is called.  ParseToken separates the command from the 
 * parameters and puts all the arguments into a char**.  (ex. cd test/test2)
 * would have the command as "cd", and char** tokens as { "test", "test2" }.
 * then according to the command either DoMkdir(), DoRmdir(), DoLS(), DoCd(),
 * exit(), or DoLSRecursive() is called. All of these essentially do the same thing,
 * they all traverse through the tree and then perform some operation.  
 * 
 * DoLSRecursive() calls DoLS for each node on the tree by using a preorder
 * traversal algorithm.
 *
 */
