Authors:
Li Chieh En
Teoh Shu Hwai
Huang Po Yu
Wang Chien Hung



# snakegame-bny-micro-controller
We use GPIO of STM32F051R8T6 to drive the LCD-00710 to show the snake game and read in the input of four buttons which are used to control the direction of the snake. When the game is playing, we also use DAC of STM32F051R8T6 to drive a 8Ω speaker to play music and a potentialometer to adjust the volume.

1. Use the four bottons to control the direction of snake.
2. The game start with a snake with length 5, the length of
the snake will increase by 3 whenever the snake eats a dot.
3. Don't hit the wall, eat your tail, or try to let the snake go to the
direction that is opposite to the direction that the snake is going.
4. When the length of the snake grows to 20, you win the game
