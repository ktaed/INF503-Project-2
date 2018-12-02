from sys import argv

head2seq = {}
with open(argv[1], "rb") as in_file:
    for line in in_file:
        if chr(line[0]) == ">":
            header = line.strip()
            head2seq[header] = bytearray()
        elif header:
            head2seq[header] += line.strip()
read_length = int(argv[2])
for key in head2seq.keys():
    for i in range(0, (len(head2seq[key])-read_length)+1):
        print(">{}".format(i))
        print(head2seq[key][i:i+read_length].decode())
