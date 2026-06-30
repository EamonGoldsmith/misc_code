import pygame
import random
import math
import time

class vec2d():
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def mag(self):
        return math.dist((0, 0), self.tup())

    def add(self, v):
        self.x = self.x + v.x
        self.y = self.y + v.y
        return (self.x self.y)

    def sub(self, v):
        self.x = self.x - v.x
        self.y = self.y - v.y
        return (self.x, self.y)

    def norm(self):
        m = self.mag()
        self.x = self.x / m.x
        self.y = self.y / m.y
        return (self.x, self.y)

    # scalar multiply
    def scm(self, f):
        self.x = self.x * f
        self.y = self.y * f

    # scalar divide
    def scd(self, f):
        self.x = self.x / f
        self.y = self.y / f

    def tup(self):
        return (self.x, self.y)

pygame.init()

# setup
screen_width = 800
screen_height = 600
screen = pygame.display.set_mode((screen_width, screen_height))
pygame.display.set_caption("Particle Simulation")

class particle():
    def __init__(self, x, y, charge, mass):
        self.charge = charge
        self.mass = mass
        self.pos = vec2d(x, y)

    def draw(self):
        p = 0
        n = 0
        g = 0

        if self.charge > 0:
            p = self.charge
        elif self.charge < 0:
            n = abs(self.charge)
        else:
            g = self.charge

        colour = (int(25 * p), int(25 * g), int(25 * n))
        size = self.mass * 10 

        # draw a filled
        pygame.draw.circle(screen, colour, (self.x, self.y), size)
        
class simulation():
    def __init__(self, particles):
        self.particles = particles

    def populate(self, num):
        self.reset()

        for i in range(num):
            x = random.randint(0, screen_width)
            y = random.randint(0, screen_height)
            c = random.uniform(-10, 10)
            m = random.uniform(0, 1)
            self.particles.append(particle(x, y, c, m))

    def simulate(self, dt):
        # copy list
        old_particles = list(self.particles)

        for p in self.particles:
            total_d = vec2d(0, 0)

            # check every particle
            for o in old_particles:
                if p != o:
                    dist = math.dist(p.pos.tup() ,o.pos.tup())

                    # coloumb's law for force
                    force = (p.charge * o.charge) / pow(dist, 2)

                    # find unit vector between both particles
                    difference = vec2d()
                    difference = p.pos.sub(o.pos)
                    unit = difference.pos.norm()
                    
                    # scale by force
                    force_vec = unit.scm(force)

                    # apply to particle position
                    total_d.add(force_vec)

            # Newton's law for acceleration
            displacement = total_d.scd(p.mass).scm(pow(dt, 2))
            p.pos.add(displacement)

    def draw(self):
        for p in self.particles:
            p.draw()

    def reset(self):
        self.particles = []

def main():
    # create simulation
    universe = simulation([])
    universe.populate(80)

    elapsed_time = 0

    running = True
    while running:
        start_time = pygame.time.get_ticks()
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        # re-draw background
        screen.fill((0, 0, 0))

        # run simulation
        dt = elapsed_time / 20
        print(dt)
        universe.simulate(dt)
        universe.draw()

        # draw buffer to screen
        pygame.display.flip()

        # fix timestep
        elapsed_time = pygame.time.get_ticks() - start_time

main()
pygame.quit()
