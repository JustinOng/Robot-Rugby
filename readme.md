# Robot Rugby

![Overall Robot](img/overall_robot.JPG?raw=true "Overall Robot")

This robot was built for the 21st Tsuyama Robotic Contest in Tsuyama, Japan by a team of 4 people over a bit more than 3 months. The competition ran in 3 minute rounds, with each robot placing the minature rugby ball on the tees, kicking it through the goal, picking it up and repeating the entire process as many times as possible. Kicking the ball from the side tees gave more points than kicking the ball from the center tee.

### Mechanical parts

![Robot Base](img/base.JPG?raw=true "Holonomic drive")
Holonomic drive powered by 4 [NeveRest 40 motors](https://www.andymark.com/NeveRest-40-Gearmotor-p/am-2964a.htm)

![Kicker](img/kicker.JPG?raw=true "Kicker")
![Kicker Latch](img/kicker_latch.JPG?raw=true "Kicker latch mechanism")
![Kicker Latched](img/kicker_latched.JPG?raw=true "Kicker latched")
Kicker consists of an aluminium beam powered by an exercise band (like a slingshot), latched down with a [gate latch](https://www.monotaro.sg/g/00529473/) which is released by a pair of solenoids (in hindsight, a servo would be better and more reliable here as our solenoids jammed sometimes). After kicking, a Neverest 60 motor winches in the kicker, latches it against the gate latch then releases the rope again. A Hall Effect sensor detected when the kicker was latched.

![Back](img/back.JPG?raw=true "Back")
Our grabbing mechanism used a modified plate grabber with an additional servo to open the grabber and elastic bands to close the grabber. The use of elastic bands allowed some play in the grabber so that we did not have to "hold" the servo there and overheat it. We also cut power to the servo when in the closed state.

![Back Winch](img/back_winch.JPG?raw=true "Back winch")
The grabber is moved up and down by a winch pulling on a rope guided by a pair of metal drawer slides. The winch is powered by a NeveRest 60 motor. You can also see the clothes peg that we used to hold down the mechanism when folded.

![Back Partially Folded](img/back_partial_folded.JPG?raw=true "Back partially folded")
![Back Folded](img/back_folded.jpg?raw=true "Back folded")
The entire grabber mechanism is hinged and folds back to allow the robot to fit in the size restriction of 0.5m x 0.5m x 0.5m. A cable tie loop on the mechanism is clipped into a clothes peg. At the start of each round, the grabber mechanism is raised, pulling the cable tie loop out of the clothes peg and releasing the mechanism.

### Electronics

![Controller board schematics](img/main_controller_schematics.png?raw=true "Main controller schematics")
The main brain of the robot was a Teensy 3.2.

![Rat's Nest](img/overall_electronics.JPG?raw=true "Overall electronics (A big mess)")
Top Right - Board holding Teensy 3.2
Top Left - Futaba R3008
Below Futaba R3008 - [Motor Controller](https://github.com/JustinOng/Motor-Controller)
Bottom Left - Device Driver, custom board with ULN2803 and MCP23008 to drive line lasers for alignment
Bottom Right - [Pololu's Dual VNH2SP30 Motor Drivers](https://www.pololu.com/product/708)

![Solenoid Driver](img/solenoid_driver.png?raw=true "Solenoid Driver schematics")
![Power distribution](img/power_distribution.PNG?raw=true "Power distribution schematics")