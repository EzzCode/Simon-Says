# Simon Says Game

This Arduino code implements a Simon Says game using an LCD display, 4 LEDs, and a joystick. The game generates a random sequence of LEDs and challenges the player to replicate it using a joystick. The LCD displays the player's score and the high score.

## Components Used
- LiquidCrystal library for LCD display
- Joystick for user input
- LEDs for gameplay
- Arduino board with analog and digital pins

## Setup
1. Connect the LCD to pins 7 (RS), 8 (EN), and 9-12 (D4-D7).
2. Connect buttons to pins 2 (down), 3 (right), 4 (up), and 5 (left).
3. Connect joystick to analog pins A1 (X-axis) and A2 (Y-axis).
4. Ensure proper power connections.

## Game Logic
- The game starts with a brief LED display indicating the current high score and the player's score.
- The player must replicate the randomly generated sequence of LEDs arranged in top, bottom, left, and right of the joystick.
- Joystick movements are used to input the player's moves.
- If the player fails to replicate the sequence correctly, LEDs blink to indicate a loss.

## Functions
1. **setup()**: Initializes LCD, sets pin modes, and triggers the initial game state.
2. **play()**: Displays and plays the generated sequence on LEDs, updates LCD with scores.
3. **reset()**: Resets game variables for a new round.
4. **lose()**: Blinks LEDs to indicate game loss.
5. **loop()**: Main game loop, reads joystick values, controls LED outputs, and manages game flow.

## Notes
- The game utilizes Serial communication for debugging (commented out if not needed).
- Adjustments to timing delays may be needed based on the hardware used.

**Enjoy playing the Simon Says game!**
