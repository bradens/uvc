using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BTreeProject
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
            abcTests();
            Console.ReadKey();
        }

        public static void abcTests()
        {
            BTree<string> tree = new BTree<string>();
            tree.Add("D");
            tree.Add("B");
            tree.Add("F");
            tree.Add("A");
            tree.Add("C");
            tree.Add("E");
            tree.Add("G");

            Console.WriteLine(tree.Count);
            tree.Remove("B");
            Console.WriteLine(tree.Count);
            BTree<string> cloneTree = tree.Clone();
            Console.WriteLine(cloneTree.ToString());
            Console.WriteLine(tree.ToString());
            BTree<string> t1 = new BTree<string>();
            BTree<string> t2 = new BTree<string>();
            t1.Remove("12");
            BTree<string> nulltest = t1 + t2;

            t1.Add("D");
            t1.Add("B");
            t1.Add("A");
            t1.Add("C");

            t2.Add("F");
            t2.Add("E");
            t2.Add("G");

            BTree<float> t3 = new BTree<float>();
            t3.Add((float)11.23);
            t3.Add((float)1.7);

            float[] vals = new float[t3.Count + 5];
            t3.CopyTo(vals, 5);

            Console.WriteLine(t1.Count + "\n" + t2.Count);
            BTree<string> addedTree = t1 + t2;
            Console.WriteLine(addedTree.Count);
            addedTree.Clear();
            Console.WriteLine(addedTree.ToString() + addedTree.Count);

            BTree<string> asdf = addedTree + tree;
            Console.WriteLine(asdf);
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
            tree.Remove(3);
            Console.WriteLine(tree.ToString());

            BTree<double> b;
            b = new BTree<double>();
            b.Add(123.45);
            b.Add(-45.8);
            b.Add(36.1);
            b.Add(42.9);
            b.Add(234.0);
            b.Remove(36.1);
            foreach (double v in b)
                Console.Write(" {0}", v);
            Console.WriteLine();

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

            BTree<string> T2 = new BTree<string>();
            T2.Add("pg"); T2.Add("lg");
            T2.Add("asdf"); T2.Add("l");

            BTree<string> asdf = T1 + T2;
            asdf.ToString();
            IEnumerator<string> e = T1.GetEnumerator();
            while (e.MoveNext())
                Console.WriteLine(e.Current);
            e.Reset();
            while (e.MoveNext())
                Console.WriteLine(e.Current);


            Console.WriteLine(T1.ToString());

            T1.Remove("languages");
        }

        public static void nullTests()
        {
            Console.WriteLine("Running Null Tests...");
            BTree<int> intTree = new BTree<int>();
            int[] a = new int[0];
            
            if (!(intTree.ToString().Equals(EMPTY_TREE_TOSTRING)))
                failedTest("FAILED TEST: EMPTY TREE TOSTRING with " + intTree.ToString() + " = " + EMPTY_TREE_TOSTRING);
            if (intTree.Count != 0)
                failedTest("FAILED TEST: EMPTY TREE COUNT. With 0 == " + intTree.Count);
            if (intTree.Contains(1234))
                failedTest("FAILED TEST: EMPTY TREE CONTAINS.");
            intTree.CopyTo(a, 23);
            if (a.Length != 0)
                failedTest("FAILED TEST: EMPTY TREE CopyTo.");
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
