using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using BTree;

namespace BTree
{
    class Program
    {
        static void Main(string[] args)
        {
            BTree<int> tree = new BTree<int>();
            tree.Add(5);
            tree.Add(6);
            tree.Add(7);
            tree.Add(1);
            Console.WriteLine(tree.ToString());
            Console.WriteLine("Done.");
            Console.ReadKey();
        }
    }
}
