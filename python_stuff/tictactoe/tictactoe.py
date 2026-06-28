
board = [
[ 'a', 'b', 'c'],
['d', 'e', 'f'],
['g', 'g', 'i'] 
]

def check_win():
    win_found = False
    
    for i in range(3):
        a = board[i][0]
        b = board[i][1]
        c = board[i][2]

        if (a == b and b == c):
            win_found = True
        
    return win_found

print(check_win())
