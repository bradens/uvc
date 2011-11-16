/**
 * C# generic Binary search tree collection
 * Author: Braden Simpson
 * Student #: V00685500
 * Csc330 Assignment 4.
 */

using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class BTree<T> : ICollection<T> where T : IComparable<T>
{
    BTreeNode root;

    /**
     * BTreeNode is the generic node class used by the 
     * BTree.
     */
    class BTreeNode
    {
        public BTreeNode leftChild;
        public BTreeNode rightChild;
        public T value;

        public BTreeNode() { }

        public BTreeNode(BTreeNode left, BTreeNode right, T val)
        {
            value = val;
            leftChild = left;
            rightChild = right;
        }
    }

    public BTree() { }

    public IEnumerator<T> GetEnumerator()
    {
        yield return root.value;
        yield break;
    }
  
    IEnumerator IEnumerable.GetEnumerator()
    {
        return this.GetEnumerator();
    }

    public void Add(T x)
    {
        //TODO
    }

    public void Clear()
    {
        //TODO
    }

    public bool Contains(T x)
    {
        //TODO
        return false;
    }

    public void CopyTo(T[] array, int arrayIndex)
    {
        //TODO
    }

    public bool Remove(T x)
    {
        //TODO
        return false;
    }

    public int Count {
        //TODO
        get {
            return -1;
        }
    }

    public bool IsReadOnly {
        get {
            return false;
        }
    }
}
