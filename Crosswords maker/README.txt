-------------------------------------------
|                GENERAL                  |
-------------------------------------------
A class named ColorMaster was created from the functions provided by JAS to deal with console color changing.

In the second program, the Puzzle class represents the game being played, managing the interaction between player, dictionary and board.

-------------------------------------------
|               CWCREATOR                 |
-------------------------------------------

The program is finished and runs without any problems, all the objectives were accomplished.
Besides what was asked to do in the project specifications, the program has many other features.
There are also some options that were adapted, according to my understanding, to better interact with the user.

ADAPTED FEATURES:

- User input against the Position? question can either be lowercase or uppercase, and not necessarily uppercase to indicate the line.
This was made to improve the interaction with the user, as it may be annoying for a user familiarized with the program to guarantee that
the input is well formated, slowing down the process. The program interprets correctly wheter it is upper or lowercase.

- Program only asks for board number and not board file name, since the board files are saved in the format bXXX.txt.

- You can remove a word from any position it occupies, not necessarily from its starting position.


NEW FEATURES:
(list may or may not be full, there are a lot of things)

- Program deals with all the odd cases, allowing (until proven wrong :D) for any thinkable board as long as all the words belong to the 
dictionary used. A good amount of time was taken in order to test all cases.

- There is a new mode that allows the user to generate a random board, without needing to do anything. There will be the option to change
the randomly created board. The user may opt for a faster and less extensive operation, or for a slower and extensive board generation.

- During the random board generation, in order to keep the user interested, there is a system that displays riddles and its answers afterwards.

- There are 2 different modes to edit the board after it has been created / resumed, strict and free.

	Strict mode: Before inserting any word a full validity test is made, not allowing the board to ever be invalid, even during word 
	insertion. 

	Free mode: You may insert any word as long as it exits, has not been used and fits the specified cells. Words intersected are not
 	taken in account, it is the user that has to have it in account. 

	Note that you may only save the board if is fully valid, in both modes.

- You may ask for full program instructions during the options menu.

- Options for the Position question:
	- I : displays simplified instructions for this question.
	- B : displays the current board.
	- R : randomly completes the board.
	- CTRL-Z : asks the user if he wants to stop editing the board.

- Options for the Word question:
	- I : displays simplified instructions for this question.
	- B : displays the current board.
	- - : removes a word in the board.
	- ? : helps the user, showing a list of words which can be placed there. Varies with edit mode.
	- + : verifies if an automatically formed word is valid.
	- # : inserts an hash.
	- R : randomly inserts a word in the specified position.
	- < : returns back to the Position question.


-------------------------------------------
|               CWPLAYER                  |
-------------------------------------------

The program is finished and runs without any problems, all the objectives were accomplished.
Besides what was asked to do in the project specifications, the program has many other features.
There are also some options that were adapted, according to my understanding, to better interact with the user.

ADAPTED FEATURES:

- User input against the Position? question can either be lowercase or uppercase, and not necessarily uppercase to indicate the line.
This was made to improve the interaction with the user, as it may be annoying for a user familiarized with the program to guarantee that
the input is well formated, slowing down the process. The program interprets correctly wheter it is upper or lowercase.

- Program only asks for board number and not board file name, since the board files are saved in the format bXXX.txt.

- '?' option displays the initial clue list and not a new clue. To display a new clue, user should use '+'.


NEW FEATURES:
(list may or may not be full, there are a lot of things)

- You may ask for full program instructions during the options menu.

- Number of wrong submissions by the player is kept as a statistic.

- After player ends the game, his statistics are shown in the console before being saved in a file.

- If player wrongly submits a board, the incorrect words are shown to the user.

- If player decides to stop playing, he may decide to see the solutions.

- Options for the Position question:
	- I : displays simplified instructions for this question.
	- S : to submit the current board, if board is full.
	- B : displays the current board.
	- ? : displays the initial clue list.
	- CTRL-Z : asks the user if he wants to stop playing.

- Options for the Word question:
	- I : displays simplified instructions for this question.
	- B : displays the current board.
	- - : removes a word in the board.
	- ? : displays the initial clue list.
	- + : shows an extra hint for the word in the specified position.
	- < : returns back to the Position question.