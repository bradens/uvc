using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using BTree;

namespace BTree
{
    class Program
    {
        public static string EMPTY_TREE_TOSTRING = "()";
        public static bool testPass = true;

        static void Main(string[] args)
        {
            intTests();
            nullTests();
            exampleTests();
            Console.ReadKey();
        }

        public static void intTests()
        {
            BTree<int> tree = new BTree<int>();
            for (int i = 10, j = 1;j < 6; i--,j++)
            {
                tree.Add(i);
                tree.Add(j);
            }
            Console.WriteLine(tree.ToString());
            Console.WriteLine(tree.Count);
            Console.WriteLine(tree.Contains(7).ToString());
            int[] arr = new int[100];
            tree.CopyTo(arr, 0);
        }

        public static void exampleTests()
        {
            BTree<string> T1 = new BTree<string>();
            T1.Add("programming");  T1.Add("languages");
            T1.Add("provide");      T1.Add("lots");
            T1.Add("opportunity");  T1.Add("for");
            T1.Add("mistakes");

            string[] arr = new string[10];
            T1.CopyTo(arr, 0);

            Console.WriteLine(T1.ToString());
        }

        public static void nullTests()
        {
            Console.WriteLine("Running Null Tests...");
            BTree<int> intTree = new BTree<int>();
            if (!(intTree.ToString().Equals(EMPTY_TREE_TOSTRING)))
                failedTest("FAILED TEST: EMPTY TREE TOSTRING with " + intTree.ToString() + " = " + EMPTY_TREE_TOSTRING);
            if (intTree.Count != 0)
                failedTest("FAILED TEST: EMPTY TREE COUNT. With 0 == " + intTree.Count);
            if (intTree.Contains(1234))
                failedTest("FAILED TEST: EMPTY TREE CONTAINS.");
            if (testPass)
            {
                sc("g");
                Console.WriteLine("Passed all null tests");
            }
            else
            {
                sc("r"); 
                Console.WriteLine("Failed Null Tests");
            }
            sc("n");
        }

        public static void failedTest(string m)
        {
            sc("e");
            Console.WriteLine(m);
            sc("n");
            testPass = false;
        }

        public static void sc(string mode)
        {
            switch (mode)
            {
                case "e":
                    Console.ForegroundColor = ConsoleColor.Red;
                    break;
                case "n":
                    Console.ResetColor();
                    break;
                case "g":
                    Console.ForegroundColor = ConsoleColor.Green;
                    break;
            }
        }
    }
}
