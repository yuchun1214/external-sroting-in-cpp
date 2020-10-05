
if __name__ == '__main__':
    numbers = []
    with open('rand.txt', 'r') as f:
        data = f.readlines()

        numbers = [int(i) for i in data]

    with open('output.txt', 'w') as f2:
        numbers.sort()
        for num in numbers:
            f2.write(str(num) + '\n')
