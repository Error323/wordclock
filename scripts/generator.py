#!/usr/bin/python

import random
import time
import sys

NUM_POOL      = 500   # Size of the chromosome pool
NUM_PARENTS   = 15    # Parents that will mate
NUM_RANDOM    = 3     # Random parents
MUTATION_RATE = 0.01  # Mutation probability per chromosome

SIZE  = 10 # Matrix SIZE x SIZE
WORDS = ["TWAALF", "EEN", "TWEE",  "DRIE", "VIER", "VIJF",   "ZES", "ZEVEN",
         "ACHT", "NEGEN", "TIEN",   "ELF", "UUR",   "VOOR", "OVER", "KWART",
         "HALF", "JANJELLE", "MARLOES", "PUCK"]

# Tuples ([words], [valid directions], [start, stop])
ORDER = [([5,10,15],                   ['h',],        [0,  15]), 
         ([13, 14],                    ['h',],        [15, 25]), 
         ([16,],                       ['h',],        [23, 29]), 
         ([0,1,2,3,4,5,6,7,8,9,10,11], ['h','v','d'], [30, 96]),
         ([12,],                       ['h',],        [97, 99]),
         ([17,18,19],                  ['h','v','d'], [0,  99])]

# Unique characters in list of words
UNIQUE = list(set("".join(WORDS)))

class Chromosome:

    def __init__(self):
        self.matrix = None
        self.random()
        self.count, self.value = self.fitness()

    def __str__(self):
        s = ""
        for i,c in enumerate(self.matrix):
            if i % SIZE == 0 and i > 0:
                s += "\n"
            s += c + ' '
        return s

    def random(self):
        self.matrix = ['.']*SIZE*SIZE

        for T in ORDER:
            random.shuffle(T[0])
            for wid in T[0]:
                d = random.choice(T[1]) # Select random direction
                w = WORDS[wid]          # Word
                l = len(w)              # Length of word

                # Horizontal
                if d == 'h':
                    at = random.randint(T[2][0], T[2][1]-l+1)
                    self.matrix[at:at+l] = w

                # Vertical
                elif d == 'v':
                    at = random.randint(T[2][0], T[2][1])
                    x = at % SIZE
                    y = at / SIZE
                    for i in range(l):
                        at = (i+y) * SIZE + x
                        if at > SIZE*SIZE-1:
                            break
                        self.matrix[at] = WORDS[wid][i]

                # Diagonal
                else:
                    at = random.randint(T[2][0], T[2][1])
                    x = at % SIZE
                    y = at / SIZE
                    for i in range(l):
                        at = (i+y) * SIZE + (i+x)
                        if at > SIZE*SIZE-1:
                            break
                        self.matrix[at] = WORDS[wid][i]


    def find(self, word, dirs, begin, end):
        length = len(word)
        count = 0

        # horizontal
        if 'h' in dirs:
            for i in range(begin, end+1):
                x = i % SIZE
                y = i / SIZE
                c = 0
                for j in range(length):
                    if x+j >= SIZE:
                        break

                    at = y*SIZE + (x+j)
                    if self.matrix[at] == word[j]:
                        c += 1
                count = max(count, c)

        # vertical
        if 'v' in dirs:
            for i in range(begin, end+1):
                x = i % SIZE
                y = i / SIZE
                c = 0
                for j in range(length):
                    if y+j >= SIZE:
                        break

                    at = (y+j)*SIZE + x
                    if self.matrix[at] == word[j]:
                        c += 1
                count = max(count, c)

        # diagonal
        if 'd' in dirs:
            for i in range(begin, end+1):
                x = i % SIZE
                y = i / SIZE
                c = 0
                for j in range(length):
                    if x+j >= SIZE or y+j >= SIZE:
                        break

                    at = (y+j)*SIZE + (x+j)
                    if self.matrix[at] == word[j]:
                        c += 1
                count = max(count, c)

        return count

    def fitness(self):
        count = 0
        for T in ORDER:
            for wid in T[0]:
                count += self.find(WORDS[wid], T[1], T[2][0], T[2][1])

        return count, 100*count+self.matrix.count('.')

    def copyfrom(self, other):
        self.count = other.count
        self.value = other.value
        for i,x in enumerate(other.matrix):
            self.matrix[i] = x


def GenerateOffspring(pool, parents):
    # Copy the best offspring as parent for the next generation
    total = 0
    best = parents[0]
    bestval = parents[0].value
    pool[0].copyfrom(parents[0])

    for child in pool[1:]:
        p1 = random.choice(parents)
        p2 = random.choice(parents)
        x1 = random.randint(0, SIZE-2)
        x2 = random.randint(x1+1, SIZE-1)
        y1 = random.randint(0, SIZE-2)
        y2 = random.randint(y1+1, SIZE-1)

        for i in range(0, SIZE):
            for j in range(0, SIZE):
                if random.random() < MUTATION_RATE and child.matrix[i*SIZE+j] == '.':
                    child.matrix[i*SIZE+j] = random.choice(UNIQUE)
                elif i >= y1 and i <= y2 and j >= x1 and j <= x2:
                    child.matrix[i*SIZE+j] = p1.matrix[i*SIZE+j]
                else:
                    child.matrix[i*SIZE+j] = p2.matrix[i*SIZE+j]

        child.count, child.value = child.fitness()
        if child.value > bestval:
            best = child
            bestval = child.value

        total += child.value
    parents[0].copyfrom(best)
    return total
            


def SelectParents(pool, parents, total_value):
    # NOTE: First parent is always the best
    for i in range(NUM_RANDOM):
        parents[i+1].random()
    rands = [random.randint(0,total_value-1) for i in range(NUM_PARENTS-NUM_RANDOM-1)]
    rands.sort()
    i = 0
    v = 0
    for c in pool:
        v += c.value
        if rands[i] < v:
            parents[i+1+NUM_RANDOM].copyfrom(c)
            i += 1
        if i == len(rands):
            break

def GenerateChromosomes(N):
    pool = []
    total_value = 0
    for i in range(N):
        pool.append(Chromosome())
        total_value += pool[i].value

    return pool, total_value
            

if __name__ == "__main__":
    pool, total_value = GenerateChromosomes(NUM_POOL)
    parents, _ = GenerateChromosomes(NUM_PARENTS)

    max_score = 0
    for T in ORDER:
        max_score += sum(map(len, [WORDS[i] for i in T[0]]))

    i = 0
    t = time.time()
    best = parents[0]
    while True:
        SelectParents(pool, parents, total_value)
        total_value = GenerateOffspring(pool, parents)

        if i % 5 == 0 and i > 0:
            tnow = time.time()
            mps = NUM_POOL / (tnow - t)
            mps *= 5.0
            t = tnow
            print "\n[{}] {} {}/{} {:0.1f} mp/s".format(i, best.value,
                                                      best.count, max_score,
                                                      mps)
            print best
            sys.stdout.flush()

        if best.count == max_score:
            print "\n[{}] found!".format(i)
            print best
            print list(set("".join(best.matrix)))
            break

        i += 1

