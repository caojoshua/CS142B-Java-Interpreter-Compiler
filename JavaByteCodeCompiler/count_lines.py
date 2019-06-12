import os

num = 0
for path in os.listdir():
	if os.path.isfile(path):
		f = open(path, "r")
		num += f.read().count("\n")
		f.close()
print(num)