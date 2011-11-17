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
        public BTreeNode parent;
        public BTreeNode leftChild;
        public BTreeNode rightChild;
        public T value;

        public BTreeNode() { }

        public BTreeNode(BTreeNode left, BTreeNode right, BTreeNode p, T val)
        {
            value = val;
            leftChild = left;
            rightChild = right;
            parent = p;
        }
    }

    public BTree() {
        root = null;
    }

    class BTreeEnumerator : IEnumerator<T>
    {
        private BTreeNode cN;
        private BTree<T> t;
        private T max;
        private int i;

        public BTreeEnumerator(BTree<T> tree) { t = tree; }

        public bool MoveNext()
        {
            if (i >= t.Count) return false;
            if (cN == null)
            {
                cN = t.findLeftMost(t.root);
                max = (new BTreeNode().value = cN.value);
            }
            else if (cN.rightChild != null)
            {
                cN = t.findLeftMost(cN.rightChild);
                max = t.getMaxNode(max, cN.value);
            }
            else if (cN.leftChild != null && cN.rightChild == null)
            {
                // find the nearest value that's bigger than our max.
                cN = t.findNearestMax(cN, max);
                max = t.getMaxNode(max, cN.value);
            }
            else
            {
                cN = cN.parent;
                max = t.getMaxNode(max, cN.value);
            }
            i++;
            return true;
        }

        public T Current
        {
            get
            {
                return cN.value;
            }
        }

        object IEnumerator.Current
        {
            get
            {
                return cN.value;
            }
        }

        public void Dispose()
        { }

        public void Reset()
        {
            cN = null;
            i = 0;
            max = default(T);
        }
    }

    private BTreeNode findNearestMax(BTreeNode cN, T max)
    {
        if (cN.value.CompareTo(max) > 0)
            return cN;
        else if (cN.rightChild != null && cN.rightChild.value.CompareTo(max) > 0)
            return cN.rightChild;
        else if (cN.parent != null)
            return findNearestMax(cN.parent, max);
        else
            return null;
        return cN;
    }

    private T getMaxNode(T v1, T v2)
    {
        if (v1 == null || v2 == null)
            throw new Exception();
        if (v1.CompareTo(v2) >= 0)
            return v1;
        else
            return v2;
    }

    private BTreeNode findLeftMost(BTreeNode cN)
    {
        if (cN.leftChild == null)
            return cN;
        else
            return findLeftMost(cN.leftChild);
    }

    public IEnumerator<T> GetEnumerator()
    {
        return new BTreeEnumerator(this);
    }
  
    IEnumerator IEnumerable.GetEnumerator()
    {
        return this.GetEnumerator();
    }

    public void Add(T x)
    {
        if (root == null)
            root = new BTreeNode(null, null, null, x);
        else
            this.insertNode(root, x);
    }

    private void insertNode(BTreeNode currNode, T x)
    {
        if (currNode == null)
            currNode = new BTreeNode(null, null, null, x);
        else if (x.CompareTo(currNode.value) <= 0)
        {
            if (currNode.leftChild == null)
                currNode.leftChild = new BTreeNode(null, null, currNode, x);
            else
                insertNode(currNode.leftChild, x);
        }
        else
        {
            if (currNode.rightChild == null)
                currNode.rightChild = new BTreeNode(null, null, currNode, x);
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
        s = array;
        copyTree(arrayIndex, root);
    }

    private static T[] s;
    private int copyTree(int aI, BTreeNode cN)
    {
        if (cN == null)
            return 0;
        else
        {
            if (cN.leftChild == null && cN.rightChild == null)
            {
                s[aI] = cN.value;
                return ++aI;
            }
            if (cN.leftChild != null)
                aI = copyTree(aI, cN.leftChild);
            s[aI] = cN.value;
            aI++;
            if (cN.rightChild != null)
                aI = copyTree(aI, cN.rightChild);
        }
        return aI;
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
