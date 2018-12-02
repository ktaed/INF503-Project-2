from sys import argv
from itertools import product
import matplotlib.pyplot as plt

def perm(chars, length):
    # s = ''.join([c * length for c in list(chars)])
    combs = product(list(chars), repeat=length)
    # print(combs)
    combs = [ ''.join(chr(x) for x in comb).encode() for comb in combs]
    combs = list(set(combs))
    combs.sort()
    return combs

def str2ind(mer):
    maps = {
        'a':"00",'c':'01', 'g':'10','t':'11'
    }
    out = b""
    for c in mer.lower():
        out =maps[chr(c)].encode() + out
    return int(out.decode(), 2)

head2seq = {}

with open(argv[1], "rb") as f:
    for line in f:
        if chr(line[0]) == ">":
            header = line.strip()
            head2seq[header] = b""
        elif header:
            head2seq[header] += line.lower().strip()

key = sorted(head2seq.keys())[0]
mer_count = {}
for i in [3,5,8,11,14]:
    mer_count = {}
    # permutes = perm(b'acgt', i)
    # for j in range(int('1'*(i*2),2 )+1):
    #     mer_count[j] = 0
        # print(j)

    for key in sorted(head2seq.keys()):
        for j in range(0, (len(head2seq[key])-i)+1):
            # str2ind(head2seq[key][j:j+i])

            try:
                mer_count[str2ind(head2seq[key][j:j+i])] += 1
            except KeyError:
                mer_count[str2ind(head2seq[key][j:j+i])] = 1
    #
    x = list(sorted(mer_count.keys()))
    y = [mer_count[m]  for m in x]
    # for l in range(int('1'*(i*2),2 )+1):
    #     x.append(l)
    #     try:
    #         y.append(mer_count[l])
    #     except KeyError:
    #         y.append(0)
    # for j in permutes:
    #     print(j, mer_count[j])
    plt.bar(x,y)
    plt.autoscale()
    # plt.xlim(0,int('1'*(i*2),2 )+2)
    plt.savefig("{}.png".format(i))
    plt.close()
    print("{}.png".format(i), len(mer_count),int('1'*(i*2),2 ))
    # print()
# with open(argv[1], "r") as f:
#     for line in f:
#         if line[0] == ">":
#
# permutations("acgt")