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

    public BTree() {
        root = null;
    }

    public IEnumerator<T> GetEnumerator()
    {
        //TODO
        yield return root.value;
        yield break;
    }
  
    IEnumerator IEnumerable.GetEnumerator()
    {
        return this.GetEnumerator();
    }

    public void Add(T x)
    {
        if (root == null)
            root = new BTreeNode(null, null, x);
        else
            this.insertNode(root, x);
    }

    private void insertNode(BTreeNode currNode, T x)
    {
        if (currNode == null)
            currNode = new BTreeNode(null, null, x);
        else if (x.CompareTo(currNode.value) <= 0)
        {
            if (currNode.leftChild == null)
                currNode.leftChild = new BTreeNode(null, null, x);
            else
                insertNode(currNode.leftChild, x);
        }
        else
        {
            if (currNode.rightChild == null)
                currNode.rightChild = new BTreeNode(null, null, x);
            else
                insertNode(currNode.rightChild, x);
        }
    }

    public void Clear()
    {
        root = null;
    }

    /**
     * Utility function to find a Node by given value.
     * param cN     : Current Node.
     * var tN       : Temporary Node.
     * param x      : Value to find.
     */
    private BTreeNode find(BTreeNode cN, T x)
    {
        BTreeNode tN = null;
        if (cN == null)
            return null;
        else if (cN.value.CompareTo(x) == 0)
            return cN;
        if (cN.leftChild != null)
            tN = find(cN.leftChild, x);
        if (tN == null && cN.rightChild != null)
            tN = find(cN.rightChild, x);
        return tN;
    }

    public bool Contains(T x)
    {
        if (this.find(root, x) != null)
            return true;
        else
            return false;
    }

    public void CopyTo(T[] array, int arrayIndex)
    {
        //aI = arrayIndex;
        array = copyTree(array, ref arrayIndex, root);
    }
    // buggy
    //public static int aI = 0;
    private T[] copyTree(T[] arr, ref int aI, BTreeNode cN)
    {
        if (cN == null)
            return arr;
        if (cN.leftChild == null && cN.rightChild == null)
        {
            arr[aI] = cN.value;
            return arr;
        }
        if (cN.leftChild != null)
        {
            arr = copyTree(arr, ref aI, cN.leftChild);
            if (!root.Equals(cN)) aI++;
        }
        arr[aI] = cN.value;
        aI++;
        if (cN.rightChild != null)
        {
            arr = copyTree(arr, ref aI, cN.rightChild);
        }
        return arr;
    }

    public bool Remove(T x)
    {
        //TODO
        return false;
    }

    public int Count {
        get {
            int count = getCount(root);
            return count;
        }
    }

    private int getCount(BTreeNode currNode)
    {
        if (currNode == null)
            return 0;
        else 
            if (currNode.leftChild == null && currNode.rightChild == null)
                return 1;
            else
                return((getCount(currNode.leftChild) + getCount(currNode.rightChild)) + 1);
    }

    public override string ToString()
    {
        StringBuilder strb = new StringBuilder();
        strb.Append("(");
        strb = this.constructString(strb, root);
        strb.Append(")");
        return strb.ToString();
    }


    // ** PSUEDOCODE ** 
    // start with (
    // if left is null and right is null
    //      return add val
    // if left is not null
    //      add '(' and do left
    //      add ') '        
    // add self.value
    // if right is not null
    //      add ' (' and do right
    //      add ')'
    private StringBuilder constructString(StringBuilder strb, BTreeNode currNode)
    {
        if (currNode == null)
            return strb;
        if (currNode.leftChild == null && currNode.rightChild == null)
            return strb.Append(currNode.value);
        if (currNode.leftChild != null)
        {
            strb.Append("(");
            strb = constructString(strb, currNode.leftChild);
            strb.Append(") ");
        }
        strb.Append(currNode.value);
        if (currNode.rightChild != null)
        {
            strb.Append(" (");
            strb = constructString(strb, currNode.rightChild);
            strb.Append(")");
        }
        return strb;
    }

    public bool IsReadOnly {
        get {
            return false;
        }
    }
}
