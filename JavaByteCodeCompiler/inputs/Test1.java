public class Test1 {
	public static void printInt(int value) {
		System.out.println(value);
	}
	public static void main(String[] args) {
		int x = 0;
		int y = x + 20;
		int z = y;
		if (x == 0) {
			z = 53;	
		}
		printInt(z);
	}

}
