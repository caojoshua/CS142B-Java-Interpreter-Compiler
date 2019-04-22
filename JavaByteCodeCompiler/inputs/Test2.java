public class Test2 {
	public static void printInt(int value) {
		System.out.println(value);
	}
	public static void main(String[] args) {
		int x = 0;
		int y = x + 20;
		int z = y;
		if (z > 0) {
			z = 53;	
		} else {
			z = 42;
		}
		printInt(z);
	}

}
