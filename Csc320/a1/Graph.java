/*
 * 
 * IMPORTANT NOTE:
 * The adjacency matrix of the graph must be symmetric.
 * If you want to add an edge (u, v) you should use the
 * TWO statements:
 *
 * G.Adj[u][v]= 1;
 * G.Adj[v][u]= 1;
 *
 */
public class Graph
{
   int n;
   int [][] Adj;

   public Graph()
   {
   }
   public static void main(String [] args)
   {
      Graph G;
      String [] answer={"NO ", "YES"};
      int count;
      int hp, hc, hp_new, hc_new;
   
      G= new Graph();

      count=0;
      while (G.read_graph())
      {
          count++;
          System.out.println("Input graph: " + count);
          G.print_graph();

          hp     = Hamilton.hamilton_path(G);
          hp_new = Hamilton.new_hamilton_path(G);
          if (hp != hp_new)
          {
             System.out.println("Error- new HP routine disagrees with old one.");
          }

          hc     = Hamilton.hamilton_cycle(G);
          hc_new = Hamilton.new_hamilton_cycle(G);
          if (hc!= hc_new)
          {
             System.out.println("Error- new HC routine disagrees with old one.\n");
          }
   
          System.out.println("Hamilton Path:  " + answer[hp]);
          System.out.println("Hamilton Cycle: " + answer[hc]);
          System.out.println();
      }
   }
   boolean read_graph()
   /* 
    *  Read in n, the number of vertices followed by an upper triangular
    *  adjacency matrix and store as adjacency matrix in G.
    */
   {
      int i, j;
      boolean ok;
      char c;
   
      n= Keyboard.readInt();
      if (n<=0) return(false);

      Adj= new int[n][n];

      for (i=0; i < n; i++)
      {
         Adj[i][i]=0;
         for (j=i+1; j < n; j++)
         {
              ok = false;
              while (!ok) 
              {
                 c = Keyboard.readChar();
                 if (c == '0') 
                 {
                     Adj[i][j]= 0;
                     ok = true;
                 }
                 else if (c == '1') 
                 {
                     Adj[i][j]= 1;
                     ok = true;
                 }
                 else if (c== Character.MIN_VALUE)
                 {
                    System.out.println(
                       "Error when trying to read position "
                       + i + "," + j + " of the adjacency matrix.");
                    return(false);
                 }
             }
             Adj[j][i]= Adj[i][j];
         }
      }
      return(true);
   }
   /* 
    *  Print the graph G in adjacency list format. 
    */
   void print_graph()
   {
      int i, j;
   
      for (i=0; i <n; i++)
      {
         print_int(i);
         System.out.print(": ");
         for (j=0; j <n; j++)
         {
             if (Adj[i][j]>=1)
             {
                print_int(j);
             }
         }
         System.out.println();
      }
   }
   void print_int(int x)
   {
      if (x < 10) System.out.print(" ");
      System.out.print(x + " ");
   }
}
