// File: scanner.cs
//
// Author: Nigel Horspool; September 2011
//

using System;
using System.IO;
using System.Text.RegularExpressions;


// The lexical elements of the SM language
public enum Token {
        Error, Eof, Ident, DecNum, FltNum, String,
        KwdVar, KwdFunc, KwdIf, KwdElse, KwdWhile, KwdReturn, KwdNull,
        OpPlus, OpMinus, OpStar, OpDivide, OpAssign,
        OpNot, OpEq, OpNe, OpLt, OpLe, OpGt, OpGe,
        LPar, RPar, LBrace, RBrace, Comma, Semicolon, Period
    }


// The scanner class contains methods for tokenizing a file
// or a string which contains source code for the SM language.
// The possible set of tokens is enumerated in the Token enum type.
public class Scanner {
    string wholeFile = null;
    string filename;
    int lineNum = 0;
    int offset = 0;
    int fileLength = 0;
    char ch = '\n';
    int tokStart = 0;
    Regex fltPattern1, fltPattern2;

    // Constructs a scanner where the source code is obtained from a file
    public Scanner( string filename ) {
        try {
            StreamReader sr;
            using(sr = new StreamReader(filename)) {
                wholeFile = sr.ReadToEnd();
                initialize();
            }
        } catch( IOException e ) {
            Console.WriteLine("Unable to read file {0}:\n\t{1}",
                filename, e.Message);
            throw new ApplicationException();
        }
        this.filename = filename;
    }

    // Constructs a scanner where the source code is obtained from a string
    // in memory; this method is intended for testing purposes.
    // The source string is argument ts; the parameter source provides a
    // name to be used in error messages.
    public Scanner( string source, string ts ) {
        wholeFile = ts;
        filename = source;
        initialize();
    }

    void initialize() {
        fileLength = wholeFile.Length;
        fltPattern1 = new Regex(@"\d*(?:[eE](?:\+|\-)?\d+)?", RegexOptions.Multiline);
        fltPattern2 = new Regex(@"\d+(?:[eE](?:\+|\-)?\d+)?", RegexOptions.Multiline);
    }

    // Immediately after GetToken has returned a token, this property may be
    // accessed to retrieve the character string in the input which corresponds
    // to the token.
    public string TokenText {
        get { return wholeFile.Substring(tokStart, offset-tokStart-1); }
    }

    // This property gives the current line number in the source input.
    public int LineNum {
        get { return lineNum; }
    }

    // The property returns the file name
    public string FileName {
        get { return filename; }
    }

    // Read further through the input source and return the next token
    // of the SM language
    public Token GetToken() {
        if (offset > wholeFile.Length)
            return Token.Eof;
        try {
            skipWhiteSpace();
            tokStart = offset-1;
            if (Char.IsLetter(ch)) {
                do {
                    ch = wholeFile[offset++];
                } while(Char.IsLetterOrDigit(ch) || ch == '_');
                switch(this.TokenText) {
                case "func":    return Token.KwdFunc;
                case "var":     return Token.KwdVar;
                case "if":      return Token.KwdIf;
                case "else":    return Token.KwdElse;
                case "while":   return Token.KwdWhile;
                case "return":  return Token.KwdReturn;
                case "null":    return Token.KwdNull;
                }
                return Token.Ident;
            }
            if (Char.IsDigit(ch)) {
                do {
                    ch = wholeFile[offset++];
                } while(Char.IsDigit(ch));
                if (ch != '.')
                    return Token.DecNum;
                // use a regular expression to match the remainder
                // of this floating-point number
                Match m = fltPattern1.Match(wholeFile,offset);
                if (m.Success) {
                    string fltstr = m.ToString();
                    offset += fltstr.Length;
                    ch = wholeFile[offset++];
                    return Token.FltNum;
                }
                return Token.Error;

            }
            char lastch = ch;
            ch = wholeFile[offset++];
            switch(lastch) {
            case '"':   while(ch != '"') {
                            if (ch == '\\')
                                offset++;
                            else if (ch == '\r' || ch == '\n')
                                return Token.Error;
                            ch = wholeFile[offset++];
                        }
                        ch = wholeFile[offset++];
                        return Token.String;
            case '+':   return Token.OpPlus;
            case '-':   return Token.OpMinus;
            case '/':   return Token.OpDivide;
            case '*':   return Token.OpStar;
            case '(':   return Token.LPar;
            case ')':   return Token.RPar;
            case '{':   return Token.LBrace;
            case '}':   return Token.RBrace;
            case ',':   return Token.Comma;
            case ';':   return Token.Semicolon;
            case '=':   if (ch == '=') {
                            ch = wholeFile[offset++];
                            return Token.OpEq;
                        }
                        return Token.OpAssign;
            case '<':   if (ch == '=') {
                            ch = wholeFile[offset++];
                            return Token.OpLe;
                        }
                        return Token.OpLt;
            case '>':   if (ch == '=') {
                            ch = wholeFile[offset++];
                            return Token.OpGe;
                        }
                        return Token.OpGt;
            case '!':   if (ch == '=') {
                            ch = wholeFile[offset++];
                            return Token.OpNe;
                        }
                        return Token.OpNot;
            case '.':   Match m = fltPattern2.Match(wholeFile,offset-1);
                        if (m.Success) {
                            string fltstr = m.ToString();
                            offset += fltstr.Length;
                            ch = wholeFile[offset-1];
                            return Token.FltNum;
                        }
                        return Token.Period;                        
            default:    return Token.Error;
            }
        } catch( IndexOutOfRangeException ) {
            tokStart = wholeFile.Length;
            offset = tokStart + 1;
            return Token.Eof;
        }
    }

    // a private method for skipping over white spaces and comments
    void skipWhiteSpace() {
        for( ; ; ) {
            if (Char.IsWhiteSpace(ch)) {
                if (ch == '\n')
                    lineNum++;
                else if (ch == '\r') {
                    if (wholeFile[offset] == '\n')
                        offset++;
                    lineNum++;
                }
                ch = wholeFile[offset++];
                continue;
            }
            if (ch != '/' || wholeFile[offset] != '/')
                break;
            do {
                ch = wholeFile[offset++];
            } while(ch != '\n' && ch != '\r');
        }
        return;
    }

    // A method for testing the other methods in the Scanner class.
    // A test string which contains at least one example of each
    // kind of token is used.
    public static bool UnitTest(bool verbose) {
        string testString =
            @"A123 123 1.2345 123.45E-2 .12345E1
            var func if else while return null
            + - * / = == != < <= > >= ( ) { } , . ; // comment \n";
        Token[] expectedTokens = { Token.Ident, Token.DecNum,
                Token.FltNum, Token.FltNum, Token.FltNum,
                Token.KwdVar, Token.KwdFunc, Token.KwdIf, Token.KwdElse,
                Token.KwdWhile, Token.KwdReturn, Token.KwdNull,
                Token.OpPlus, Token.OpMinus, Token.OpStar, Token.OpDivide,
                Token.OpAssign,
                Token.OpEq, Token.OpNe, Token.OpLt,
                Token.OpLe, Token.OpGt, Token.OpGe,
                Token.LPar, Token.RPar, Token.LBrace, Token.RBrace,
                Token.Comma, Token.Period, Token.Semicolon, Token.Eof };
        Scanner s = new Scanner("test-string", testString);
        foreach( Token t in expectedTokens ) {
            Token tt = s.GetToken();
            if (verbose)
                Console.WriteLine("Test: token = {0}, text = '{1}'", tt, s.TokenText);
            if (t != tt) {
                Console.WriteLine("Scanner: unit test failed on token {0}", t);
                return false;
            }
        }
        Console.WriteLine("Scanner: unit test succeeded");
        return true;
    }

}
