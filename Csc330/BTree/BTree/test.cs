//// Generic linked list, nodes have both a key and an item value
//// Modified from an example found on msdn.microsoft.com

//using System;
//using System.Collections;
//using System.Collections.Generic;


//public class LinkedList<K, T> : ICollection<T> where T : IComparable<T>
//{
//    Node m_Head;

//    // Nested class
//    // K is the key, T is the data item
//    class Node
//    {
//        public K Key;
//        public T Item;
//        public Node NextNode;

//        public Node()
//        {
//            Key = default(K);
//            Item = default(T);
//            NextNode = null;
//        }

//        public Node(K key, T item, Node nextNode)
//        {
//            Key = key;
//            Item = item;
//            NextNode = nextNode;
//        }
//    }

//    public LinkedList()
//    {
//        m_Head = new Node();  // put a dummy first node in the list
//    }

//    // insert a new element at the front of the list
//    public void AddHead(K key, T item)
//    {
//        Node newNode = new Node(key, item, m_Head.NextNode);
//        m_Head.NextNode = newNode;
//    }


//    // Overload the indexing operation [] for get access only
//    // This is the public way to search the linked list for a key
//    public T this[K key]
//    {
//        get { return Find(key); }
//    }

//    // this returns a strange result if the key is not found!
//    private T Find(K key)
//    {
//        Node current = m_Head.NextNode;  // skip dummy node
//        while (current != null)
//        {
//            if (current.Key.Equals(key))
//                break;
//            current = current.NextNode;
//        }
//        if (current == null) return default(T);
//        return current.Item;
//    }

//    public IEnumerator<T> GetEnumerator()
//    {
//        Node current = m_Head.NextNode;  // skip dummy node
//        while (current != null)
//        {
//            yield return current.Item;
//            current = current.NextNode;
//        }
//        yield break;  // no more elements
//    }

//    IEnumerator IEnumerable.GetEnumerator()
//    {
//        return this.GetEnumerator();
//    }

//    // overload the + operator to mean list concatenation
//    public static LinkedList<K, T> operator +(LinkedList<K, T> lhs, LinkedList<K, T> rhs)
//    {
//        return concatenate(lhs, rhs);
//    }

//    public static LinkedList<K, T> concatenate(LinkedList<K, T> list1, LinkedList<K, T> list2)
//    {
//        LinkedList<K, T> newList = new LinkedList<K, T>();
//        Node current = list1.m_Head;
//        while (current != null)
//        {
//            newList.AddHead(current.Key, current.Item);
//            current = current.NextNode;
//        }

//        current = list2.m_Head;
//        while (current != null)
//        {
//            newList.AddHead(current.Key, current.Item);
//            current = current.NextNode;
//        }
//        return newList;
//    }

//    ////////////////////////  the following methods and properties are provided
//    ////////////////////////  only so that the ICollection<T> interface is
//    ////////////////////////  nominally implemented
//    public void Add(T x)
//    {
//        // unimplemented!
//    }

//    public void Clear()
//    {
//        // unimplemented
//    }

//    public bool Contains(T x)
//    {
//        // unimplemented
//        return false;
//    }

//    public void CopyTo(T[] array, int arrayIndex)
//    {
//        // unimplemented
//    }

//    public bool Remove(T x)
//    {
//        // unimplemented
//        return false;
//    }

//    public int Count { get { return -1; } }  // unimplemented

//    public bool IsReadOnly { get { return false; } }

//}

//public class TestTheList
//{
//    public static void Main()
//    {
//        LinkedList<string, double> people = new LinkedList<string, double>();
//        people.AddHead("fred", 100.0);
//        people.AddHead("jane", 53.5);
//        people.AddHead("bill", 84.2);

//        foreach (double wt in people)
//        {
//            Console.Write(" {0}", wt);
//        }
//        Console.WriteLine();

//        Console.WriteLine("jane's weight = {0}", people["jane"]);
//    }
//}
