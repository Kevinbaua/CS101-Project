Video Link : https://drive.google.com/drive/folders/1XrMCrzMNneoTG0Y8k0l72ChUUjGvnlRT?usp=sharing

Features : All the required features and all the extra features are implemented.

Controls : 
w - shoot bullet
a - move left
d - move right
q - quit during game
Mouse click - quit after game is over

Code : 
In the main game loop firstly event is checked and shooter is moved or bullet is released accordingly.
Then,
check_bullets_hit  ( implements bullet-bubble collisions ) 
check_shooter_hit  ( implements shooter-bubble collisions ) 
move_bubbles	  ( moves bubbles )
move_bullets	  ( moves bullets )
these functions are called.
Then an if condition to check if health is 0 or time has run out then end game.
Then an if condition to check if all bubble are destroyed then advance level or win.
Then after waiting for 0.02s loop starts over again.

	


