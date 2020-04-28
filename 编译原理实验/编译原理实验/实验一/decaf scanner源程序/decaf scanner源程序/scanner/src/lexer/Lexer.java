package lexer;

import java.io.IOException;
import java.util.Hashtable;

public class Lexer {

	public static int line = 1;
	char peek = ' ';   //һ�����ַ�����������������ı�
	Hashtable words = new Hashtable();  //��Ĭ�ϵĳ�ʼ���� (11) �ͼ������� (0.75) ����һ���µĿչ�ϣ��
	void reserve(Word w) {  // ��ϣ��������  w.lexeme Ϊ��  w Ϊֵ
		words.put(w.lexeme, w);
	}

	public Lexer() {  //��ϣ���ʼ��
		reserve(new Word("if", Tag.IF));  // �� if
		reserve(new Word("else", Tag.ELSE));  // �� else
		reserve(new Word("while", Tag.WHILE));  //�� while
		reserve(new Word("do", Tag.DO));  //�� do
		reserve(new Word("break", Tag.BREAK));  //�� break
		reserve(Word.True); //�� true
		reserve(Word.False);  //�� false
	}
	 //�����û������ַ����������ַ����� peek ��
	public void readch() throws IOException { 
		peek = (char) System.in.read();
		
	}
	//ʵ�� ��Ӵ�ƥ��ԭ�� ����Ҫ�� readch ����
	boolean readch(char c) throws IOException {
		readch();
		if (peek != c) {  //�����벻��������c �򷵻� false
			return false;
		}
		peek = ' ';  //����������c �򷵻�true ����peek ��Ϊ��
		return true;
	}

	public Token scan() throws IOException {  //ɨ���û�������ַ���
		for (;; readch()) {  //ɨ�������еĿո� �Ʊ�� ���з� �����Կո��Ʊ�������з�ʹ�������ӡ�
			if (peek == ' ' || peek == '\t')  //�����ո���Ʊ�� ���¿�ʼforѭ��
				continue;
			else if (peek == '\n') {  //�������з���������һ
				line += 1;
			} else {
				break;
			}
		}
		//�յ������ų�����ȡ�������ַ����������Ա�����ƥ��
		switch (peek) {  //�ַ���peek��Ĭ��ת��Ϊ����
		case '&':   //������������&&���ţ����뱣����andƥ��
			if (readch('&')) 
				return Word.and;
			else  //ֻ��һ��&���� 
				return new Token('&');
		case '|':  // ��&ͬ��
			if (readch('|'))  
				return Word.or;
			else
				return new Token('|');
		case '=':  // ��&ͬ��
			if (readch('='))
				return Word.eq;
			else
				return new Token('=');
		case '!':  // ��&ͬ��
			if (readch('='))
				return Word.ne;
			else
				return new Token('!');
		case '<':  // ��&ͬ��
			if (readch('='))
				return Word.le;
			else
				return new Token('<');
		case '>':  // ��&ͬ��
			if (readch('='))
				return Word.ge;
			else
				return new Token('>');
		}
		if (Character.isDigit(peek)) {  //����Character���е�isDigit()����    �ж������Ƿ�Ϊ����0~9
			int v = 0;
			do {
				v = 10 * v + Character.digit(peek, 10);  // ��peek��ֵת��Ϊʮ���ƣ�����ʮ���ƹ���Խ�����ֵ���ݴ����������ұ���ֵ�ݴε�ʮ��
				readch();
			} while (Character.isDigit(peek));  //�������������ʮ��������
			if (peek != '.')  //�� ������������û��С���㣬�򷵻� һ�� Num���͵Ķ���ɨ�������NumΪToken�����ࡣ
				return new Num(v);
			 //�������а���С���㣬������������ΪС���� һ��Ϊ��С�����ֵ�ɨ��
			float x = v; 
			float d = 10;
			for (;;) {
				readch();
				if (!Character.isDigit(peek))  //������������һ���ַ��������֣�������ѭ��
					break;
				x = x + Character.digit(peek, 10) / d;  // ʮ����С���Ĵ���
				d = d * 10;
			}
			return new Real(x);  //����һ��Real���͵Ķ���Real��ΪToken������ࡣɨ�������
		}
		//����ĸ��ͷ�������Ĵ����������������֣����ֲ��ᱻ��ȡ����
		if (Character.isLetter(peek)) {  //�ж������ַ��Ƿ�Ϊ��ĸ
			StringBuffer b = new StringBuffer();  //����һ�����ַ�����������ʼ����Ϊ16���ַ�
			do {
				b.append(peek);  //��peek�ӽ�������
				readch();
			} while (Character.isLetterOrDigit(peek));  //������������ַ��ӽ�һ��������
			String s = b.toString();  // ��������b�е����ݸ���һ��String ����s
			Word w = (Word) words.get(s);  //�����ƶ���s��ӳ���ֵ ��ǿ������ת�� ת��ΪWord���� �� ��ӳ�䲻�����˼���ӳ�� �򷵻�null
			if (w != null)  // w��Ϊnull ��ʾ ����Ϊһ���ؼ��ֻ���ֵ
				return w;  //����ɨ��
			w = new Word(s, Tag.ID);  // ���� ����Ϊһ����ͨ�ַ� ���Ϊ ID
			words.put(s, w);  // ����������ϣ��
			return w;  //����ɨ��
		}
		Token tok = new Token(peek);
		peek = ' ';
		return tok;
	}
	
	public void out() {
		System.out.println(words.size());  // ���ع�ϣ���м�������
		
	}

	public char getPeek() {
		return peek;
	}

	public void setPeek(char peek) {
		this.peek = peek;
	}

}
