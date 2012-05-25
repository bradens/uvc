public class Hamilton
{
/* These first four routines are of type int and return
   0 if there is no Hamilton cycle and
   1 if there is a Hamilton cycle 
   It would be more common in Java to use a Boolean variable
   instead but I wanted the code to work the same way as the
   C code handed out so all students would be on a common footing.
*/
   public static int new_hamilton_path(Graph G)
   {
       /* Put your new code here. */
	   
	   
	   
	   
       return(1);
   }

   public static int new_hamilton_cycle(Graph G)
   {
       /* Put your new code here. */
       return(1);
   }

   public static int hamilton_path(Graph G)
   {
       int i, j;
       int [] mask;
       int n;
   
       n= G.n;
       mask = new int[n];
   
       if (n <= 1) return(1);
   
       for (i=0; i <n; i++)
           mask[i]=1;
   
   
       for (i=0; i <n; i++)
    
       {
           for (j=i+1; j <n; j++)
           {
               if (hamuv(n, G, n, i, j, mask))
               {
                   return(1);
               }
           }
       }
       return(0);
   }
   public static int hamilton_cycle(Graph G)
   {
       int [] mask;
       int n;
       int i, j;
   
       n= G.n;
       mask = new int[n];
   
       if (n <= 2) return(0);
   
       for (i=0; i <n; i++)
           mask[i]=1;
   
       for (i=0; i <n; i++)
       {
           for (j=i+1; j <n; j++)
           {
               if (G.Adj[i][j]==1)
               {
                  if (hamuv(n, G, n, i, j, mask))
                  {
                      return(1);
                  }
               }
           }
       }
       return(0);
   }
   /* NOTE: This hamuv code is "inside the black box"
   and so you may not call it! If you call this function
   in your solution, you will not get any marks. */
   
   private static boolean hamuv(int n, Graph G, 
                 int nv, int u, int v, int [] mask)
   /* Look for a Hamilton Path from u to v. */
   {
      int i;
   
      if (nv ==2)
      {
         return(G.Adj[u][v]==1);
      }
   
      mask[u]=0;
   
      for (i=0; i <n; i++)
      {
          if ((mask[i]==1) && (G.Adj[u][i]==1) && i!= v)
          {
              if (hamuv(n, G, nv-1, i, v, mask))
              {
                  mask[u]=1;
                  return(true);
              }
          }
       }
       mask[u]=1;
       return(false);
   }
}
