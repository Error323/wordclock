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


def find(word, dirs, start, end, matrix):
    length = len(word)
    indices = []

    # horizontal
    if 'h' in dirs:
        for i in range(start, end+1):
            x = i % SIZE
            y = i / SIZE
            indices = []
            for j in range(length):
                if x+j >= SIZE:
                    break

                at = y*SIZE + (x+j)
                if matrix[at] == word[j] and at in range(start,end+1):
                    if y % 2 == 0: 
                        indices.append(at) 
                    else:
                        indices.append(y*SIZE+(SIZE-1-(x+j)))
                else:
                    break

                if len(indices) == length:
                    return indices

    # vertical
    if 'v' in dirs:
        for i in range(start, end+1):
            x = i % SIZE
            y = i / SIZE
            indices = []
            for j in range(length):
                if y+j >= SIZE:
                    break

                at = (y+j)*SIZE + x
                if matrix[at] == word[j] and at in range(start,end+1):
                    if (y+j) % 2 == 0: 
                        indices.append(at) 
                    else:
                        indices.append((y+j)*SIZE+(SIZE-1-x))
                else:
                    break

                if len(indices) == length:
                    return indices

    # diagonal
    if 'd' in dirs:
        for i in range(start, end+1):
            x = i % SIZE
            y = i / SIZE
            indices = []
            for j in range(length):
                if x+j >= SIZE or y+j >= SIZE:
                    break

                at = (y+j)*SIZE + (x+j)
                if matrix[at] == word[j] and at in range(start,end+1):
                    if (y+j) % 2 == 0: 
                        indices.append(at) 
                    else:
                        indices.append((y+j)*SIZE+(SIZE-1-(x+j)))
                else:
                    break

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

    m = max(W, key=len)
    output = [0] * SIZE*SIZE
    j = 0
    for T in ORDER:
        for w in T[0]:
            I = find(WORDS[w], T[1], T[2][0], T[2][1], matrix)
            assert(len(I) == len(WORDS[w]))
            for i in I:
                output[i] |= (1 << j)
            if j > 3 and WORDS[w] in ["VIJF","TIEN"]:
                print "static const uint32_t %s_%s= (1ul << %i);" % (WORDS[w], ' ' * (len(m)-len(WORDS[w])) ,j)
            else:
                print "static const uint32_t %s %s= (1ul << %i);" % (WORDS[w], ' ' * (len(m)-len(WORDS[w])) ,j)
            j += 1

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

