from random import randint
from random import random
from random import uniform
from math import cos
from math import pi
from copy import deepcopy
import sys

initial_range = (int(sys.argv[1]), int(sys.argv[2]))
population_size = 5
selected_size = 3
mutate_chance = 5
iterations = 5000

def initial_pop():
	return [randint(initial_range[0], initial_range[1]) for i in range(population_size)]

def fitness(candidates):
	f = []
	for c in candidates:
		fit = -(10 + ((c - 1)**2 -10 * cos(2 * pi * (c - 1))))
		f.append(fit)
	return f

def draw_index(fitness):
	maxx     = sum([f for f in fitness])
	pick    = uniform(0, maxx)
	current = 0
	for i in range(0, len(fitness)):
		current += fitness[i]
		if current > pick:
			return i

	return fitness.index(max(fitness))

def crossover(c1, c2):
	return (c1 + c2)/2

def mutate(c):
	if randint(0, 100) < mutate_chance:
		return c + random()
	return c

pop = initial_pop()
fit = fitness(pop)

for it in range(0, iterations):
	new_pop = []
	left_pop = [pop[draw_index(fit)], pop[draw_index(fit)]]
	for i in range(0, selected_size):
		index = draw_index(fit)
		del fit[index]
		index2 = draw_index(fit)
		new_pop.append(mutate(crossover( pop[index], pop[index2] )))

	new_pop.extend(left_pop)
	pop = deepcopy(new_pop)
	fit = fitness(new_pop)

print str(new_pop[fit.index(max(fit))])
