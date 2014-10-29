#!/usr/bin/python

from generator import *
import argparse

DIR = {H: "HORIZONTAL", V: "VERTICAL", D:"DIAGONAL"}
DIRINT = {H: 29, V: 30, D: 31}

def get_configuration():
    """Returns a populated configuration"""
    parser = argparse.ArgumentParser(
      description="Create matrix header file for wordclock"
    )
    parser.add_argument("--matrix", "-m",
      help="{0}x{0} matrix in column major".format(SIZE),
    )

    return parser.parse_args()


def find(word, dirs, start, end, matrix):
    """Find word in the matrix"""
    length = len(word)

    for d in dirs:
        for i in range(start, end+1):
            indices = []
            for j in range(length):
                index = i+d*j
                if index <= end and matrix[index] == word[j]:
                    indices.append(index)
                else:
                    break
                if len(indices) == length:
                    return indices, d

    raise ValueError("No indices found for ({}, {}, {}-{})".format(word, dirs, start, end))

def mat2str(M):
    s = " * "
    for i,c in enumerate(M):
        if i % SIZE == 0 and i > 0:
            s += "\n * "
        s += c + ' '
    return s


if __name__ == "__main__":
    config = get_configuration()
    alphabet = [chr(i+65) for i in range(26)]
    matrix = config.matrix
    full = [' '] * SIZE*SIZE
    for i,c in enumerate(matrix):
        if c == '.':
            full[i] = random.choice(alphabet)
        else:
            full[i] = c

    max_score = 0
    W = []
    for T in ORDER:
        max_score += sum(map(len, [WORDS[i] for i in T[0]]))
        for i in T[0]:
            W.append(WORDS[i])

    print "/**"
    print " * Autogenerated header file for the wordclock"
    print " *   {} words encoded".format(len(W))
    print " *   {}/{} empty squares\n *".format(config.matrix.count('.'), max_score)
    print mat2str(full)
    print " */\n"

    m = max(W + DIR.values(), key=len)
    output = [0] * SIZE*SIZE
    j = 0
    for T in ORDER:
        for w in T[0]:
            I, d = find(WORDS[w], T[1], T[2][0], T[2][1], matrix)
            assert(len(I) == len(WORDS[w]))
            for i in I:
                output[i] |= (1 << j)
                output[i] |= (1 << DIRINT[d])
            if j > 3 and WORDS[w] in ["VIJF","TIEN"]:
                print "static const uint32_t %s_%s= (1ul << %i);" % (WORDS[w], ' ' * (len(m)-len(WORDS[w])) ,j)
            else:
                print "static const uint32_t %s %s= (1ul << %i);" % (WORDS[w], ' ' * (len(m)-len(WORDS[w])) ,j)
            j += 1

    for k,v in DIR.iteritems():
        print "static const uint32_t %s %s= (1ul << %i);" % (v, ' ' * (len(m)-len(v)), DIRINT[k])

    s = ""
    for i,c in enumerate(output):
        if i % SIZE == 0 and i > 0:
            s += "\n"
        if i < len(output)-1:
            s += str(c) + ','
        else:
            s += str(c)

    print "\nstatic const uint32_t hours[] = {"
    for w in WORDS[:11]:
        c = ""
        if w in ["TIEN", "VIJF"]:
            c = "_"
        print "%s%s," % (w, c)
    print "%s\n};" % (WORDS[11])

    print "\nstatic const uint32_t matrix[] = {"
    print s
    print "};\n"

