
# load
piecesall = ["pixels1", "pixels2", "pixels3"]
names = ["king", "pawn", "rook"]
board = ["pawn", "pawn", "pawn", "king"]

converter = {}
for p, b in zip(piecesall, names):
	converter[b] = p

# draw board
for i in board:
	print(converter[i])