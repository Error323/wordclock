#!/usr/bin/python

import random

NUM_POOL      = 500   # Size of the chromosome pool
NUM_PARENTS   = 15    # Parents that will mate
NUM_RANDOM    = 3     # Random parents
MUTATION_RATE = 0.01  # Mutation probability per chromosome

SIZE  = 10 # Matrix SIZE x SIZE
WORDS = ["TWAALF", "EEN", "TWEE",  "DRIE", "VIER", "VIJF",   "ZES", "ZEVEN",
         "ACHT", "NEGEN", "TIEN",   "ELF", "UUR",   "VOOR", "OVER", "KWART",
         "HALF", "JANJELLE", "MARLOES"]

# Tuples ([words], [valid directions], [start, stop])
ORDER = [([5,10,15],                   ['h',],        [0,  15]), 
         ([13, 14],                    ['h',],        [15, 24]), 
         ([16,],                       ['h',],        [24, 29]), 
         ([0,1,2,3,4,5,6,7,8,9,10,11], ['h','v','d'], [30, 95]),
         ([12,],                       ['h',],        [96, 99]),
         ([17,18],                     ['h','v','d'], [0,  99])]

# Unique characters in list of words
UNIQUE = list(set("".join(WORDS)))

class Chromosome:

    def __init__(self):
        self.matrix = None
        self.random()
        self.value = self.fitness()

    def __str__(self):
        s = ""
        for i,c in enumerate(self.matrix):
            if i % SIZE == 0 and i > 0:
                s += "\n"
            s += c + ' '
        return s

    def random(self):
        self.matrix = [random.choice(UNIQUE) for i in range(SIZE*SIZE)]

        for T in ORDER:
            random.shuffle(T[0])
            for wid in T[0]:
                d = random.choice(T[1]) # Select random direction
                w = WORDS[wid]          # Word
                l = len(w)              # Length of word

                # Horizontal
                if d == 'h':
                    at = random.randint(T[2][0], T[2][1]-l)
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

        # horizontal
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
        f = 0
        for T in ORDER:
            for wid in T[0]:
                f += self.find(WORDS[wid], T[1], T[2][0], T[2][1])

        return f

    def copyfrom(self, other):
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
        crossover = random.randint(0, SIZE*SIZE-1)
        for i in range(crossover):
            if random.random() < MUTATION_RATE:
                child.matrix[i] = random.choice(UNIQUE)
            else:
                child.matrix[i] = p1.matrix[i]
        for i in range(crossover+1, SIZE*SIZE):
            if random.random() < MUTATION_RATE:
                child.matrix[i] = random.choice(UNIQUE)
            else:
                child.matrix[i] = p2.matrix[i]
        child.value = child.fitness()
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

    i = 0
    while True:
        SelectParents(pool, parents, total_value)
        total_value = GenerateOffspring(pool, parents)
        if i % 5 == 0:
            print "\n[{}] fitness: {}".format(i, parents[0].value)
            print parents[0]
        i += 1

