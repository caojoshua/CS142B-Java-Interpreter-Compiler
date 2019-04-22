public class Test4 {
	public static void printInt(int value) {
		System.out.println(value);
	}

	public static void main(String[] args) {
		int sum = 0;
		int i = 0;
		while (i < 5) {
			sum += i;	
			i++;
		}

		printInt(sum);
	}

}
