public class Test5 {
	public static void printInt(int value) {
		System.out.println(value);
	}

	public static void main(String[] args) {
		int sum = 0;
		int i = 0;
		while (i < 10) {
			if (i == 5) {
				sum += i;	
			} else {
				sum += 25;
			}
			i++;
		}

		printInt(sum);
	}

}
