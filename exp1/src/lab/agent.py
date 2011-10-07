import random
class Agent(object):
    def __init__(self):
        self.met = random.randint(1, 12)
        self.acq = random.randint(120, 200)
        self.vis = random.randint(1, 30)

    @property
    def scouter(self):
        return self.vis * 1000 + (12 - self.met) * 1000 + self.acq

    @property
    def turn(self):
        turn = (4000 * self.met - 5000) / self.acq
        if turn <= 0:
            return 100
        return turn
