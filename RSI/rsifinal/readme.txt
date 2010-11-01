/* 
 * $ Header: rsi.c rsi.h
 * $ Author: Braden Simpson
 * $ Student Number: V00685500
 * $ Date : October 5 2010
 * 
 * =========================================================================
 * 
 * Real Shell Interpreter
 * Programming Assignment 1
 * Csc 360
 *
 * This program works by accepting line input from the user, calling
 * tokenizeLine() on it, which creates a string array of  all the arguments
 * separated by a " ".  Then the command entered by the user is separated into
 * either an internal command or external command.  Internal commands include 
 * "cd, bg,  exit".  External commands are any executable by default in the 
 * $PATH or by the path entered by the user.  When bg is called a new Process 
 * Node(PNode*) is made and connected to a Process List.  Then when a process is
 * finished, stopped, killed, or started, the PNode* reflects this.  When the 
 * user enters bglist, the RSI iterates through all the PNodes in the Process
 * list and prints the order of creation, the status, and the command.  
 * 
 * When exit() is called, the program destroys the structs  it allocated and
 * frees it back to the system.
 *
 */
