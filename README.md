# Tic Tac Toe
This C++ program uses the WinSock2 libaray to create a UDP scoket between two computers in order to send the location of the respected players marker (player 1 is 'X' and player 2 is 'O').  

 
 One key feature of the Tic Tac Toe Program is that the program has automatic checks in place to see if there is a winner, loser and a tie, and sends the appropriate message to the console
 
 Another key feature is that Tic Tac Toe will automatically quit if a user takes to long to place a marker (this can be changed in the wait function call, by deafult is set to 20 seconds and 0 milliseconds). If the user takes to long to place a piece then the user is alerted that he has lost and the opponent has won the match by deafult, with respected message to conosle of respeted player.
 
 Another key feature is that Tic Tac Toe has limits on what input can we accepeted, only ints can be accepted and ints only in the range of 1-9 (places on game board). When a user inputs a invailid input the user will be prompted that the input was not accept and will ask for another input.
 
 Another key feature is that Tic Tac Toe will kepp track of who is playing who by logging the IP address of the player and when the program recives a UDP datagram it will compare it to the IP address in a varible in the program and if the IP address match Tic Tac Toe will continue to place marker on game board.
 
 ## How To Run Rational
  1. Clone this repo by pasting `git clone https://github.com/Joshua-Sutton/Tic-Tac-Toe.git` into terminal
  2. Open and new Visual Studio project 
  3. Import the files in this repository `playTicTacToe.cpp`, `TicTacToe.cpp`, `Utilities.cpp`, and `TicTacToe.h`
  4. Run `Crtl + F5` 
  5. Input a name in the `What is your name` input
  6. Select `1` to host a Tic Tac Toe game
  7. Open another command propmt via `Crtl + F5' 
  8. Select `2` to challenge someone alrealy hosting a Tic Tac Toe game
  9. Then you will need to input your IP address in the `What is your IP address: ` box
  10. Then you will need to input your subnet in the `What is your subnet address: ` box
  11. Then type `y` to accept challenge 
  12. Input a number 1 - 9 when prompted with `Where would you like to place marker: `
  13. Keep placing markers untill you see `TIE ... boo` indicating a tie or `You lose` indicating you lost or `YOU WIN` indicating that you won the game
  14. Then select `1` if you like to host again, or select `2` if you would like to challenge again or select `3` to quit the program
  
  ## Example 
  
  ### Clinet Side (Challenger)
  ```
What is your name? player2

Choose an option:
 1 - Host a game of Tic-Tac-Toe
 2 - Challenge someone else
 3 - Quit
Enter 1, 2 or 3: 2

Looking for Tic Tac Toe servers...
What is your IP address? 10.35.165.41
What is your subnet mask? 255.255.240.0
Found Tic Tac Toe server: player1

Do you want to challenge player1? y
     |     |
  1  |  2  |  3
_____|_____|_____
     |     |
  4  |  5  |  6
_____|_____|_____
     |     |
  7  |  8  |  9
     |     |
You are X's.
Where would you like to place piece? 5
  ```
  
  ### Server Side (Host)
  ```
  What is your name? player1

Choose an option:
 1 - Host a game of Tic-Tac-Toe
 2 - Challenge someone else
 3 - Quit
Enter 1, 2 or 3: 1

Waiting for a challenge...

Received: Who?
Sent: Name=player1

Received: Player=player2

You have been challenged by player2
     |     |
  1  |  2  |  3
_____|_____|_____
     |     |
  4  |  5  |  6
_____|_____|_____
     |     |
  7  |  8  |  9
     |     |
... waiting for opponet ...
  ```
