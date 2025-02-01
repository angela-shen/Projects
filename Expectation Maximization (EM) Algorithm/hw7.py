#!/usr/bin/env python
# coding: utf-8


import numpy as np


ratings = []
for i in open('hw7_ratings-1.txt', 'rt'):
    ratings.append(np.array(i.split()))
ratings = np.array(ratings)
movies = []
for i in open('hw7_movies-1.txt', 'rt'):
    movies.append(i.strip())



probs = []
for i in open('hw7_probR_init.txt', 'rt'):
    probs.append(np.array(i.split()).astype(float))
probs = np.array(probs)
pids = []
for i in open("hw7_ids-2.txt"):
    pids.append(i.strip())


# part a
ratings_1 = np.array([sum('1' == i) for i in ratings.T])
ratings_0 = np.array([sum('0' == i) for i in ratings.T])
ratings_q = np.array([sum('?' == i) for i in ratings.T])
r = ratings_1 / (ratings_1 + ratings_0)
movie_mean = [[r[i], movies[i]] for i in range(len(r))]
movie_mean.sort()
# produces latex code for me so I don't have to screenshot: 
#for i in range(len(movie_mean)):
#    print(str(movie_mean[i][0]) + ", " + movie_mean[i][1] + "\\\\")
movie_mean



# part e
def log_likelihood(d, z, probs):
    L = 0
    for t in range(len(d)):
        r_t = ratings[t]
        r_j = 0
        for i in range(4):
            prod = 1
            for j in range(60):
                if r_t[j] == '?':
                    continue
                elif r_t[j] == '1':
                    prod *= probs[j, i]
                elif r_t[j] == '0':
                    prod *= (1 - probs[j, i])
            r_j += z[i] * prod
        L += np.log(r_j)
    return L / (len(d))

# m step
# me on my way to invent the most suboptimal code
def update(d, z, probs):
    # T x k matrix of all rho_it
    pits = []
    for t in range(len(d)):
        r_t = ratings[t]
        for_t = []
        for i in range(4):
            prod = 1
            for j in range(60):
                if r_t[j] == '?':
                    continue
                elif r_t[j] == '1':
                    prod *= probs[j, i]
                elif r_t[j] == '0':
                    prod *= (1 - probs[j, i])
            for_t.append(z[i] * prod)
        pits.append(np.array(for_t) / sum(for_t))
    pits = np.array(pits)
    # all summation pit for all i
    summation_pit = np.sum(pits, axis = 0)
    # divide by T
    z = summation_pit / len(d)
    rj_z = []
    for j in range(60):
        nums = []
        for i in range(4):
            numer = 0
            denom = 0
            for t in range(len(d)):
                r_t = ratings[t]
                denom += pits[t, i]
                if r_t[j] == '0':
                    continue
                elif r_t[j] == '?':
                    numer += pits[t, i] * probs[j, i]
                elif r_t[j] == '1':
                    numer += pits[t, i] 
            nums.append(numer / denom)
        rj_z.append(np.array(nums))
    #print(summation_pit)
    return z, np.array(rj_z)



z = [0.25, 0.25, 0.25, 0.25]
p = probs
print('0 | ' + str(log_likelihood(ratings, [0.25, 0.25, 0.25, 0.25], probs)))
for i in range(1, 257):
    z, p = update(ratings, z, p)
    if i in [2 ** i for i in range(0, 9)]:
        print(str(i) + " | " + str(log_likelihood(ratings, z, p)))
#b
#a, b


r_t = ratings[pids.index('A18083527')]


for r in range(len(r_t)):
    zis = []
    rl_zs = []
    for i in range(4):
        prod = 1
        for j in range(60):
            if r_t[j] == '?':
                continue
            elif r_t[j] == '1':
                prod *= probs[j, i]
            elif r_t[j] == '0':
                prod *= (1 - probs[j, i])
        zis.append(z[i] * prod)
        rl_zs.append(p[r, i])
    if r_t[r] == "?":
        # formatted to 
        print(str(r) + ": " + movies[r]  + ", " + str(sum((np.array(zis) / sum(zis)) * rl_zs)) + "\\\\")




