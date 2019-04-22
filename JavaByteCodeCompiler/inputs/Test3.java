public class Test3 {
	public static void printInt(int value) {
		System.out.println(value);
	}
	public static void main(String[] args) {
		int sum = 0;
		for (int i = 0; i < 5; i++) {
			sum += i;	
		}

		printInt(sum);
	}

}
