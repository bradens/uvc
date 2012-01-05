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
    private BTreeNode root;
    private int nodeCount;
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
        this.nodeCount = 0;
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

    /**
     * Finds the nearest maximum valued node on the given 
     * subtree rooted at cN
     */
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
    }

    /*
     * Returns the greater T value.
     */
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

    private BTreeNode findRightMost(BTreeNode cN)
    {
        if (cN.rightChild == null)
            return cN;
        else
            return findRightMost(cN.rightChild);
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
        this.nodeCount += 1;
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
        this.nodeCount = 0;
    }

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
        foreach (T x in this)
        {
            array[arrayIndex] = x;
            arrayIndex++;
        }
    }

    public bool Remove(T x)
    {
        if (this.removeRec(x, root))
        {
            this.nodeCount--;
            return true;
        }
        else
            return false;
    }

    private bool removeRec(T x, BTreeNode cN)
    {
        if (cN == null)
            return false;
        if(x.CompareTo(cN.value) < 0)
            return removeRec(x, cN.leftChild);
        else if (x.CompareTo(cN.value) > 0)
            return removeRec(x, cN.rightChild);
        else
        {
            if (cN.leftChild != null && cN.rightChild != null)
            {
                Random s = new Random();
                int balancer = s.Next(2);
                BTreeNode child;

                if (balancer > 0)
                    child = findRightMost(cN.leftChild);
                else
                    child = findRightMost(cN.rightChild);

                // Store temp value
                T tmp = child.value;

                // remove the child.
                removeRec(child.value, root);

                // swap childs value into cN
                cN.value = tmp;
            }
            else if (cN.leftChild != null && cN.rightChild == null)
                this.deleteChild(cN, cN.leftChild);
            else if (cN.leftChild == null && cN.rightChild != null)
                this.deleteChild(cN, cN.rightChild);
            else
                this.deleteChild(cN, null);
            return true;
        }
    }

    private bool deleteChild(BTreeNode child, BTreeNode newNode)
    {
        if (child.parent != null)
        {
            if (child.parent.leftChild != null && child.parent.leftChild.Equals(child))
                child.parent.leftChild = newNode;
            else if (child.parent.rightChild != null && child.parent.rightChild.Equals(child))
                child.parent.rightChild = newNode;
            return true;
        }
        else if (this.root.Equals(child))
        {
            root = newNode;
            return true;
        }
        return false;
    }

    public BTree<T> Clone()
    {
        BTree<T> newTree = new BTree<T>();
        if (this.root == null)
            return newTree;
        // add the root first
        newTree.Add(this.root.value);
        newTree = this.recClone(newTree, this.root, this.findRightMost(root));
        return newTree;
    }

    // recursively add nodes to the clone.  Add the two children, then call on left child.
    // then on right child.  Always produces exact clone of the given tree.
    private BTree<T> recClone(BTree<T> currTree, BTreeNode cN, BTreeNode rN)
    {
        if (cN == null || cN.Equals(rN))
            return currTree;
        if (cN.leftChild != null)
            currTree.Add(cN.leftChild.value);
        if (cN.rightChild != null)
            currTree.Add(cN.rightChild.value);
        currTree = recClone(currTree, cN.leftChild, rN);
        currTree = recClone(currTree, cN.rightChild, rN);
        return currTree;
    }

    public static BTree<T> operator +(BTree<T> T1, BTree<T> T2)
    {
        BTree<T> newTree = new BTree<T>();
        if (T1.root == null)
            return T2;
        if (T2.root == null)
            return T1;
        if (T1.root.value.CompareTo(T2.root.value) < 0)
        {
            newTree.Add(T1.root.value);
            newTree.Add(T2.root.value);
        }
        else
        {
            newTree.Add(T2.root.value);
            newTree.Add(T1.root.value);
        }
        newTree = recAddTrees(newTree, T1.root, T2.root);
        return newTree;
    }

    /// <summary>
    /// Recursively adds two BTrees together by adding both the children of each
    /// node before recursing on the left child, then right child in parallel for both
    /// trees.
    /// </summary>
    /// <param name="newTree">current Tree to return.</param>
    /// <param name="cN1">current node for Tree1</param>
    /// <param name="cN2">currrent node for Tree2</param>
    /// <returns>A new BTree </returns>
    private static BTree<T> recAddTrees(BTree<T> newTree, BTreeNode cN1, BTreeNode cN2)
    {
        if (cN1 == null && cN2 == null)
            return newTree;
        if (cN1 != null && cN1.leftChild != null)
            newTree.Add(cN1.leftChild.value);
        if (cN2 != null && cN2.leftChild != null)
            newTree.Add(cN2.leftChild.value);
        if (cN1 != null && cN1.rightChild != null)
            newTree.Add(cN1.rightChild.value);
        if (cN2 != null && cN2.rightChild != null)
            newTree.Add(cN2.rightChild.value);
        newTree = recAddTrees(newTree, cN1 != null ? cN1.leftChild : null, cN2 != null ? cN2.leftChild : null);
        newTree = recAddTrees(newTree, cN1 != null ? cN1.rightChild : null, cN2 != null ? cN2.rightChild : null);
        return newTree;
    }

    public int Count {
        get {
            return this.nodeCount;
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
