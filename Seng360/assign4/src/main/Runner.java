package main;

public class Runner {
  public static void main(String[] args) {
    try {
      if (args[0].equals("1"))
        Part1.runPart1();
      else if (args[0].equals("2"))
        Part2.runPart2();
      else
        System.out.println("Usage java -jar assign4 [1 | 2]");
    }
    catch (Exception e) {
      e.printStackTrace();
      return;
    }
  }
}
