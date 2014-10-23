#!/usr/bin/python

import random
import time
import sys

NUM_POOL      = 1000   # Size of the chromosome pool
NUM_PARENTS   = 15     # Parents that will mate
NUM_RANDOM    = 3      # Random parents
MUTATION_RATE = 0.001  # Mutation probability per chromosome

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


class Chromosome:
    def __init__(self, i, j):
        self.i = i
        self.j = j
        self.word = WORDS[ORDER[i][0][j]]
        self.length = len(self.word)
        self.startpts = {'v':[], 'h':[], 'd':[]}

        for s in range(ORDER[i][2][0], ORDER[i][2][1]+1):
            x = s % SIZE
            y = s / SIZE
            if x <= SIZE-self.length:
                self.startpts['h'].append(s)
            if y <= SIZE-self.length:
                self.startpts['v'].append(s)
            if x <= SIZE-self.length and y <= SIZE-self.length:
                self.startpts['d'].append(s)

        self.random()

    def random(self):
        self.direction = random.choice(ORDER[self.i][1])
        self.start = random.choice(self.startpts[self.direction])

    def __str__(self):
        return "({}, {} {})".format(self.word, self.start, self.direction)

    def copyfrom(self, other):
        self.i = other.i
        self.j = other.j
        self.word = other.word
        self.length = other.length
        self.direction = other.direction


class Gene:
    def __init__(self):
        self.chromosomes = []
        for i, T in enumerate(ORDER):
            for j,_ in enumerate(T[0]):
                self.chromosomes.append(Chromosome(i, j))

        self.matrix = ['.']*SIZE*SIZE
        self.random()
        self.count, self.value = self.fitness()


    def __str__(self):
        s = ""
        for i,c in enumerate(self.matrix):
            if i % SIZE == 0 and i > 0:
                s += "\n"
            s += c + ' '
        return s


    def createmat(self):
        for c in self.chromosomes:
            # Horizontal
            if c.direction == 'h':
                x = c.start % SIZE
                y = c.start / SIZE
                for i in range(c.length):
                    if x > SIZE-1:
                        break
                    self.matrix[y*SIZE+x] = c.word[i]
                    x += 1

            # Vertical
            elif c.direction == 'v':
                x = c.start % SIZE
                y = c.start / SIZE
                for i in range(c.length):
                    if y > SIZE-1:
                        break
                    self.matrix[y*SIZE+x] = c.word[i]
                    y += 1

            # Diagonal
            else:
                x = c.start % SIZE
                y = c.start / SIZE
                for i in range(c.length):
                    if x > SIZE-1 or y > SIZE-1:
                        break
                    self.matrix[y*SIZE+x] = c.word[i]
                    x += 1
                    y += 1


    def random(self):
        for c in self.chromosomes:
            c.random()
        self.createmat()
            

    def find(self, c):
        count = 0

        # Horizontal
        if c.direction == 'h':
            x = c.start % SIZE
            y = c.start / SIZE
            for i in range(c.length):
                if x > SIZE-1:
                    break
                count += 1 * (self.matrix[y*SIZE+x] == c.word[i])
                x += 1

        # Vertical
        elif c.direction == 'v':
            x = c.start % SIZE
            y = c.start / SIZE
            for i in range(c.length):
                if y > SIZE-1:
                    break
                count += 1 * (self.matrix[y*SIZE+x] == c.word[i])
                y += 1

        # Diagonal
        else:
            x = c.start % SIZE
            y = c.start / SIZE
            for i in range(c.length):
                if x > SIZE-1 or y > SIZE-1:
                    break
                count += 1 * (self.matrix[y*SIZE+x] == c.word[i])
                x += 1
                y += 1

        return count


    def fitness(self):
        count = 0
        for c in self.chromosomes:
            count += self.find(c)
        return count, 100*count+self.matrix.count('.')


    def copyfrom(self, other):
        self.count = other.count
        self.value = other.value
        for i,x in enumerate(other.matrix):
            self.matrix[i] = x
        for i,c in enumerate(other.chromosomes):
            self.chromosomes[i].copyfrom(c)



def GenerateOffspring(pool, parents):
    # Copy the best offspring as parent for the next generation
    total = 0
    best = parents[0]
    bestval = parents[0].value
    pool[0].copyfrom(parents[0])

    for child in pool[1:]:
        p1 = random.choice(parents)
        p2 = random.choice(parents)

        for i,c in enumerate(child.chromosomes):
            r = random.random()
            if r < MUTATION_RATE:
                c.random()
            elif r < 0.5:
                c.copyfrom(p1.chromosomes[i])
            else:
                c.copyfrom(p2.chromosomes[i])
        
        child.createmat()
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

def GenerateGenes(N):
    pool = []
    total_value = 0
    for i in range(N):
        pool.append(Gene())
        total_value += pool[i].value

    return pool, total_value
            

if __name__ == "__main__":
    pool, total_value = GenerateGenes(NUM_POOL)
    parents, _ = GenerateGenes(NUM_PARENTS)

    max_score = 0
    for T in ORDER:
        max_score += sum(map(len, [WORDS[i] for i in T[0]]))

    i = 0
    g = 0
    t = time.time()
    best = parents[0]
    while True:
        SelectParents(pool, parents, total_value)
        total_value = GenerateOffspring(pool, parents)

        g += 1
        n = time.time() - t
        if n > 5.0:
            mps = g*NUM_POOL / n
            t += n
            g = 0
            print "[{}] {} {}/{} {:0.1f} mp/s".format(i, best.value,
                                                      best.count, max_score,
                                                      mps)
            sys.stdout.flush()

        if best.count == max_score:
            print "\n[{}] found!".format(i)
            print best
            print list(set("".join(best.matrix)))
            break

        i += 1

