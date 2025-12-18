import pygame
from pygame.locals import *

pygame.init()
w, h = 800, 600

screen = pygame.display.set_mode((w, h))

running = True
while running:
	for event in pygame.event.get():
		if event.type == pygame.event.QUIT:
			running = False

	img = pygame.image.load('test_image.jpg')
	img.convert()

	screen.blit(img, [0, 0, 200, 200])

	pygame.display.update()

pygame.quit()