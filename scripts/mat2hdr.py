#!/usr/bin/python

from generator import *
import argparse

def get_configuration():
    """Returns a populated configuration"""
    parser = argparse.ArgumentParser(
      description="Create matrix header file for wordclock"
    )
    parser.add_argument("--matrix", "-m",
      help="{0}x{0} matrix in column major".format(SIZE),
      default="JKWARTIEN.AVIJFMVOORNOVERAHALFJ.T.DRIEVNETWAALFPIELAEZ.O.UJGLCEEEENCFEEH.V.S.K.N.TIENVIER.ELFN...UUR")

    return parser.parse_args()


def find(word, matrix):
    length = len(word)
    indices = []

    # horizontal
    for i in range(SIZE*SIZE):
        x = i % SIZE
        y = i / SIZE
        indices = []
        for j in range(length):
            if x+j >= SIZE:
                break

            at = y*SIZE + (x+j)
            if matrix[at] == word[j]:
                indices.append(at)

        if len(indices) == length:
            return indices

    # vertical
    for i in range(SIZE*SIZE):
        x = i % SIZE
        y = i / SIZE
        indices = []
        for j in range(length):
            if y+j >= SIZE:
                break

            at = (y+j)*SIZE + x
            if matrix[at] == word[j]:
                indices.append(at)

        if len(indices) == length:
            return indices

    # diagonal
    for i in range(SIZE*SIZE):
        x = i % SIZE
        y = i / SIZE
        indices = []
        for j in range(length):
            if x+j >= SIZE or y+j >= SIZE:
                break

            at = (y+j)*SIZE + (x+j)
            if matrix[at] == word[j]:
                indices.append(at)

        if len(indices) == length:
            return indices

    raise ValueError("No indices found for {}".format(word))

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

    print "/**"
    print mat2str(full)
    print " */\n"

    W = []
    for T in ORDER:
        for i in T[0]:
            W.append(WORDS[i])

    K = {}
    m = max(W, key=len)
    for i, w in enumerate(W):
        K[w] = 1 << i
        if i > 3 and w in ["VIJF","TIEN"]:
            print "#define %s_%s (1ul << %i)" % (w, ' ' * (len(m)-len(w)) ,i)
        else:
            print "#define %s %s (1ul << %i)" % (w, ' ' * (len(m)-len(w)) ,i)

    output = [0] * SIZE*SIZE
    for w in W:
        I = find(w, matrix)
        for i in I:
            output[i] |= K[w]

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

