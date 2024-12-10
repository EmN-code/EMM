lines = [
    "122122121122212",
    "444434444333434",
    "222222222222222",
    "111211111111122",
    "222222222222222"
]


total_sum = 0

for line in lines:
    total_sum += sum(int(char) for char in line)

print(total_sum/180.0)