# 🤖 IntroductionToRobotics Repository - 2023-2024

Welcome to my Introduction to robotics repository! This repository includes all the projects and assignments I've worked on during this course.

## 📁 Repository Contents

- **Homework Assignments**: In this section, you can find my homework assignments including requirements and implementation details.

- **Project Work**: Detailed project descriptions, code, and resources can be found in the respective project folders.

- **Resources**: The 'Resources' directory includes helpful documents, links, and references related to this course.

- **Images and Media**: In this section you can explore images, diagrams, and videos used for my work.

## 📚 Homeworks

### Homework 1 - RGB Led controlled by potentiometers

- **Task requirement**: This assignment focuses on controlling each channel (Red, Green, and Blue)of  an  RGB  LED  using  individual  potentiometers.
- **Description**: This implementation involves three main steps: Find the limit of each potentiometer, adding the RGB LED to the circuit and finalizing the code. [Check here for in-depth information](https://github.com/mihaitufescu/IntroductionToRobotics/blob/main/Homeworks/Homework%20%231%20-%20RGB%20led%20controlled%20by%20potentiometer/implementation.md)

Watch the demo:
https://www.youtube.com/shorts/h66co6thW88

### Homework #2 - Elevator simulator

- **Task requirement**: This  assignment  involves  simulating  a  3-floor  elevator  control  system  usingLEDs, buttons, and a buzzer with Arduino. Follow me in order to gain experience in using button state change, implementing debouncing techniques,and coordinating multiple components to represent real-world scenarios.
- **Description**: This project effectively integrates button control with elevator operation, creating a functional elevator simulation. Users can initiate elevator calls and track its floor-to-floor movement using LEDs to represent floor levels. Auditory feedback is provided through a buzzer. Notably, the code is structured with separate methods, improving code readability and maintainability. It serves as a practical example of how to implement an interactive project with a well-organized code structure. [Check here for in-depth information](https://github.com/mihaitufescu/IntroductionToRobotics/blob/main/Homeworks/Homework%20%232%20-%20Elevator%20simulator/implementation.md)
  
### Homework #3 - 7 segment display drawing

- **Task requirement**: Use the joystick to control the position ofthe segment and ”draw” on the display.  The movement between segments should be natural, meaning they should jump from the current positiononly to neighbors, but without passing through ”walls”.
- **Description**: The project efficiently handles button clicks and debounce, ensuring that button presses are accurately detected. It distinguishes short clicks from long presses, resetting the display when a button is held for 3 seconds. The code separates the logic neatly, with clear segments for button handling and joystick input. It's efficient in managing LED states, enabling blinking animations at regular intervals. The joystick input determines segment changes, with smooth transitions. The code is well-structured, making it easy to understand and maintain, while effectively controlling an 8-segment display with a button and joystick for a satisfying user experience. [Check here for in-depth information](https://github.com/mihaitufescu/IntroductionToRobotics/blob/main/Homeworks/Homework%20%233%20-%207%20segment%20display%20drawing/implementation.md)

Watch the demo:
https://www.youtube.com/shorts/P6K85QJXZc8

### Homework #4 - Stopwatch

- **Task requirement**: Using the 4 digit 7 segment display and 3 buttons, you should implement a stopwatch timer that counts in 10ths of a second and has a save lap functionality (similar to most basic stopwatch functions on most phones).
- **Description**: The project is a countdown timer with 4-digit 7-segment displays and buttons control. It employs a debouncing mechanism for start/stop, reset, and lap buttons. The LapStack class maintains lap times in a circular stack, allowing for continuous storage of the latest lap times. The program also handles display initialization, activation, and writing numbers to the displays. The start/stop button toggles countdown activation, and the reset button clears the count unless the timer is running. The flag button records lap times. The code ensures stable button readings and seamless display of countdown and lap data. [Check here for in-depth information](https://github.com/mihaitufescu/IntroductionToRobotics/blob/main/Homeworks/Homework%20%234%20-%20Stopwatch/implementation.md)

Watch the demo:
https://youtu.be/0sVqS57-NV4
