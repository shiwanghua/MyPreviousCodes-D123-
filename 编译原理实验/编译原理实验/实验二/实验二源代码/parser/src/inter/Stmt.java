package inter;
//�ڵ��һ������  ���

public class Stmt extends Node {

   public Stmt() { }

   public static Stmt Null = new Stmt();

   public void gen(int b, int a) {} // called with labels begin and after

   int after = 0;                // saves label after  Ĭ��Ϊ public
   public static Stmt Enclosing = Stmt.Null;  // used for break stmts
   
   public void display() {}   //��ʾ
}
