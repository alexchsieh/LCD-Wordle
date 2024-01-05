# Hangman Game with Arduino and Joystick

## Introduction

This project is an implementation of the classic Hangman game using Arduino and a joystick for user interaction. The game utilizes a LiquidCrystal display for the UI and a buzzer for audio feedback. The player navigates through the alphabet using the joystick to make guesses and has a limited number of attempts to guess the correct word.

## Hardware Requirements

- Arduino board
- Joystick module
- LiquidCrystal display
- Buzzer
- Push button switch
- Resistors and wires

## Setup

1. Connect the hardware components according to the provided pin configurations in the code.
2. Upload the provided Arduino code to your board.
3. Power on the Arduino board.

## How to Play

1. The game starts with the main screen displaying the current choices and the number of attempts left.
2. Use the joystick to navigate through the alphabet and make a selection by pressing the joystick button.
3. Move to the "Letters Screen" by pressing the switch when the cursor is on the desired letter.
4. Guess the word by forming a sequence of letters. Confirm your guess by navigating to the last column and pressing the switch.
5. The game will provide feedback on correct and incorrect guesses.
6. The game ends when the player either correctly guesses the word or runs out of attempts.

## Game States

- **Main Screen:** Displays the current choices and attempts left. Use the joystick to navigate.
- **Letters Screen:** Allows the player to select letters for their guess.
- **Win/Lose:** Displayed when the game is won or lost. Press the switch to reset the game.

## Audio Feedback

- A win is accompanied by a victory sound played on the buzzer.
- A loss is accompanied by a losing sound played on the buzzer.

## Contributing

Feel free to contribute to the project by submitting pull requests or reporting issues.

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgments

- [LiquidCrystal Library](https://www.arduino.cc/en/Reference/LiquidCrystal)
- [Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary)
