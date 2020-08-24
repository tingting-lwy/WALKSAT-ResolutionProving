import random


def posneg(probability):
    return random.random() < probability


file = open("CNF.txt","r+") 
file.truncate(0)

file.write("c k-CNF (m,n) randomly generated clauses\n")

# k  of literals in each clause, n is number of symbols, m is number of clauses
k, n , m =0,0,0
n = random.randint(2,100)
m = random.randint(1,50)

file.write("p cnf "+str(n)+ " " +str(m)+ " \n")
lines = [None] * m

for i in range(m):
    k = random.randint(1,3)
    symbols = [i for i in range(1, n+1)]
    string = ""

    for j in range(k):
        if not symbols:
            break
        literal = random.choice(symbols)
        symbols.remove(literal)
        if posneg(0.5):
            literal = "-"+str(literal)

        string += str(literal)
        string+= ' '
    lines[i] = string
lines = [i for n, i in enumerate(lines) if i not in lines[:n]]
print(len(lines))
while len(lines) < m:
    difference = m - len(lines)

    for i in range(difference):
        k = random.randint(1,3)
        symbols = [i for i in range(1, n+1)]
        string = ""

        for j in range(k):
            if not symbols:
                break
            literal = random.choice(symbols)
            symbols.remove(literal)
            if posneg(0.5):
                literal = "-"+str(literal)

            string += str(literal)
            string+= ' '
        lines.append(string)
    lines = [i for j, i in enumerate(lines) if i not in lines[:j]]


print(lines)

for i in range(len(lines)):
    file.write(lines[i]+ "0\n")

file.close()

file = open("CNF.txt","r+") 
lines = file.readlines()
lines = lines[:-1]
# file.write("")
file.close()