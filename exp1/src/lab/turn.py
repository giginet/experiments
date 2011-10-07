from agent import Agent
agents = [Agent() for i in xrange(500)]
turns = [a.turn/a.vis for a in agents]
#print len([a.turn/a.vis for a in agents if a.turn > 330])
t = 100
#print max(turns)*t
#print (sum(turns)/5000)*t
#print min(turns)*t

print len([a for a in agents if a.vis < 2 or a.met > 10])
