// File: bytecode.cs
//
// This code uses C# 4.0 properties
//
// Author: Nigel Horspool; September 2011
//

using System;
using System.Collections.Generic;


public enum OpCode {
    Load, Store, LoadIntConst, LoadFltConst, LoadStrConst, LoadNull,
    Jump, JumpIfFalse, Call, Pop, Return,
    Add, Sub, Mul, Div, CmpEq, CmpNe, CmpGt, CmpGe, CmpLt, CmpLe,
    Read, Write, WriteLine, Stop }


// A class for an instruction which has no immediate operands
public class Instruction {
    OpCode op;

    public Instruction( OpCode op ) {
        this.op = op;
    }

    public OpCode Op { get{ return op; } }

    public override string ToString() {
        return String.Format("{0}", Op);
    }
}

// A class for an instruction which has a single integer as an immediate operand
public class InstructionInt: Instruction {
    public int IntOperand { get; set; }

    public InstructionInt( OpCode op, int opnd ): base(op) {
        IntOperand = opnd;
    }

    public override string ToString() {
        return String.Format("{0}\t{1}", Op, IntOperand);
    }
}

// A class for an instruction which has a single floating-point
// number as an immediate operand
public class InstructionFlt: Instruction {
    float fltOperand;

    public InstructionFlt( OpCode op, float opnd ): base(op) {
        this.fltOperand = opnd;
    }

    public float FltOperand { get{ return fltOperand; } }

    public override string ToString() {
        return String.Format("{0}\t{1}", Op, FltOperand);
    }
}

// A class for an instruction which has a single string as an immediate operand
public class InstructionStr: Instruction {
    string strOperand;

    public InstructionStr( OpCode op, string opnd ): base(op) {
        this.strOperand = opnd;
    }

    public string StrOperand { get{ return strOperand; } }

    public override string ToString() {
        return String.Format("{0}\t{1}", Op, StrOperand);
    }
}

// A special class used only for the call instruction
public class InstructionStrInt: Instruction {
    string name;
    int numArgs;

    public InstructionStrInt( OpCode op, string name, int numArgs ): base(op) {
        this.name = name;  this.numArgs = numArgs;
    }

    public string Name { get{ return name; } }

    public int NumArgs { get{ return numArgs; } }

    public override string ToString() {
        return String.Format("{0}\t{1}  // arity {0}", Op, Name, NumArgs);
    }
}

// An instance of this class is used to contain the bytecode for
// a single function in the SM language.
// The class also records the number of parameters expected by the
// function, plus the names of all the parameters and local variables
// declared anywhere in the function.
public class CompiledFunction {
    string name;
    int numArguments;
    int numLocals;  // the number of local variables, including the arguments!
    IList<string> locals;
    IList<Instruction> code;

    // Construct a new instance of the class for a function with the
    // name provided
    public CompiledFunction( string name ) {
        this.name = name;
        numArguments = 0;
        locals = new List<string>();
        code = new List<Instruction>();
    }

    public string Name {
        get { return name; }
    }
    
    public int NumArguments {
        get { return numArguments; }
    }

    public int NumLocals {
        get { return numLocals; }
    }

    public Instruction GetInstructionAt( int pc ) {
        try {
            return code[pc];
        } catch( Exception ) {
            throw new ApplicationException(
                String.Format("attempt to fetch instruction at invalid offset {0}", pc));
        }
    }

    // Add one more argument to the list of function arguments
    public int AddArgument( string name ) {
        locals.Add(name);
        numLocals++;
        return numArguments++;
    }

    // Add one more variable to the list of local variables
    public int AddVariable( string name ) {
        locals.Add(name);
        return numLocals++;
    }

    // Add one more instruction to the list of bytecode instructions
    public int AddInstruction( OpCode op ) {
        Instruction ins = new Instruction(op);
        code.Add(ins);
        return code.Count-1;
    }

    // Add one more instruction to the list of bytecode instructions
    public int AddInstruction( OpCode op, int opnd ) {
        Instruction ins = new InstructionInt(op, opnd);
        code.Add(ins);
        return code.Count-1;
    }

    // Add one more instruction to the list of bytecode instructions
    public int AddInstruction( OpCode op, float opnd ) {
        Instruction ins = new InstructionFlt(op, opnd);
        code.Add(ins);
        return code.Count-1;
    }

    // Add one more instruction to the list of bytecode instructions
    public int AddInstruction( OpCode op, string opnd ) {
        Instruction ins = new InstructionStr(op, opnd);
        code.Add(ins);
        return code.Count-1;
    }

    // Add one more instruction to the list of bytecode instructions
    public int AddInstruction( OpCode op, string opnd, int num ) {
        Instruction ins = new InstructionStrInt(op, opnd, num);
        code.Add(ins);
        return code.Count-1;
    }

    // Replaces the immediate integer operand of the instruction at
    // the specified offset with a new value    
    public void PatchInstruction( int offset, int opnd ) {
        InstructionInt inst = code[offset] as InstructionInt;
        inst.IntOperand = opnd;
    }

    // Returns the position of the next position in the bytecode array
    public int getCodeOffset() {
        return code.Count;
    }

    // Prints details of the function and prints its bytecode 
    public void DumpBytecode() {
        Console.WriteLine("\n*** Function {0}:\n*** Arguments\n\t\t", name);
        int i = 0;
        if (numArguments > 0)
            while(i < numArguments) {
                Console.Write("{0}  ", locals[i++]);
            }
        else
            Console.Write("-- none --");
        Console.Write("\n*** Local Variables\n\t\t");
        if (numLocals > numArguments)
            while(i < numLocals) {
                Console.Write("{0}  ", locals[i++]);
            }
        else
            Console.Write("-- none --");
        Console.WriteLine("\n*** Bytecode");
        for( i = 0;  i < code.Count;  i++ ) {
            Instruction ins = code[i];
            Console.WriteLine("{0}:\t{1}", i, ins);
        }
        Console.WriteLine();
    }

    // A method for testing the other methods in the CompiledFunction class.
    public static bool UnitTest(bool verbose) {
        try {
            CompiledFunction cf = new CompiledFunction("test");
            cf.AddArgument("a");
            cf.AddArgument("b");
            cf.AddVariable("x");
            cf.AddVariable("y");
            cf.AddInstruction(OpCode.Load,1);
            int k = cf.AddInstruction(OpCode.Jump, 0);
            cf.AddInstruction(OpCode.Call, "getval", 0);
            int t = cf.getCodeOffset();
            cf.PatchInstruction(k, t);
            cf.AddInstruction(OpCode.Add);
        } catch( Exception e ) {
            Console.WriteLine("CompiledFunction: unit test failed; exception thrown:\n\t{0}",
                e.Message);
            return false;
        }
        Console.WriteLine("CompiledFunction: unit test succeeded");
        return true;
    }

}
