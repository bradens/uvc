// File: symboltable.cs
//
// Author: Nigel Horspool;  September 2011

using System;
using System.Collections.Generic;

// This class contains methods for tracking the names of variables
// which are visible at the current point in the SM program.
public class SymbolTable {
    IList<IDictionary<string,int>> visibleVariables;
    int scopeLevel;
    IDictionary<string,int> top;

    // Construct a new empty symbol table
    public SymbolTable() {
        visibleVariables = new List<IDictionary<string,int>>();
        scopeLevel = -1;
        StartScope();
    }

    // Called when a new scope level begins -- this is usually
    // when a left curly brace is seen in the input
    public void StartScope() {
        top = new Dictionary<string,int>();
        visibleVariables.Add(top);
        scopeLevel++;
    }

    // Called when a scope level end -- this is usually
    // when a right curly brace is seen in the input    
    public void EndScope() {
        visibleVariables.RemoveAt(scopeLevel--);
        top = visibleVariables[scopeLevel];
    }

    // Attempts to add an entry for the variable with the
    // given name to the list of names defined in the current scope.
    // If the name is already there, a result of false is returned.
    // Otherwise, the name is added to the list and true is returned.
    // The second parameter, offset, is a number to be remembered with
    // the name; offset refers to a slot within the function's frame
    // when the SM program executes.
    public bool AddEntry( string name, int offset ) {
        if (top.ContainsKey(name)) {
            return false;
        }
        top[name] = offset;
        return true;
    }

    // This method prints all the entries in the symbol table
    public void DumpEntries() {
        Console.WriteLine("\nSymbol Tables Entries (level {0})",
            visibleVariables.Count);
        foreach( string name in top.Keys ) {
            Console.WriteLine("\tframe offset {0}: {1}", top[name], name);
        }
    }

    // Looks up the number associated with the name in the symbol table.
    // If the name cannot be found, the result is -1.
    public int LookupOffset( string name ) {
        int result = -1;
        for( int depth = scopeLevel;  depth >= 0;  depth-- ) {
            IDictionary<string,int> symbols = visibleVariables[depth];
            if (symbols.TryGetValue(name, out result))
                return result;
        }
        return -1;
    }
}
