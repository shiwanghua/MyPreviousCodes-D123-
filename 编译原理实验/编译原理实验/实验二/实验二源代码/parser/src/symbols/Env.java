package symbols;
//ʵ���������ӷ��ű�
import inter.Id;

import java.util.Hashtable;

import lexer.Token;

public class Env {
	private Hashtable table;  // �洢���ű�
	protected Env prev;  //ָ����һ�����ű�
	
	public Env(Env n) { table = new Hashtable(); prev=n;}  //����һ���µķ��ű�
	
	public void put(Token w, Id i) {  //�ڵ�ǰ���ű���д��һ����Ŀ
		table.put(w, i);
	}
	
	public Id get(Token w){  // �õ�һ����ʶ������Ŀ
		for(Env e=this; e!=null; e=e.prev){  //�ӵ�ǰ���ſ�ʼ����
			Id found = (Id)(e.table.get(w));  //�ҵ��󷵻ظ���Ŀ
			if(found!=null) return found;  //�Ҳ���������null
		}
		return null;
	}
}
