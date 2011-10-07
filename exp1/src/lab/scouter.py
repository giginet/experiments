import random
import itertools
from agent import Agent
AGENTS = 500
agents = [Agent() for i in xrange(AGENTS)]
print sum([agent.scouter for agent in agents])/AGENTS
print [i for i in itertools.ifilter(lambda a: a.scouter < 1000, agents)]
