package inter;

public class Seq extends Stmt {    //   一个语句序列

	   Stmt stmt1; Stmt stmt2;

	   public Seq(Stmt s1, Stmt s2) { stmt1 = s1; stmt2 = s2; }

	   public void gen(int b, int a) {
		   if(stmt1==Stmt.Null) stmt2.gen(b, a);
		   else if(stmt2==Stmt.Null) stmt1.gen(b, a);
		   else {
			   int label=newlabel();
			   stmt1.gen(b, label);
			   emitlabel(label);
			   stmt2.gen(label, a);
		   }
	   }
	   
	   public void display(){
		  if ( stmt1 == Stmt.Null ) stmt2.display();
		     else if ( stmt2 == Stmt.Null ) stmt1.display();
		     else {
		    	stmt1.display();
		        stmt2.display();
		     }
	   }
	}

