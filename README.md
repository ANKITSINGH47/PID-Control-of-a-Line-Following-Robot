# PID Control of a Line Following Robot
Design and implement a PID controller for the Spark V robot to make it follow a continuous track (papers with printed sample lines will be provided) on the ground, using the IR sensors provided on the robot for this purpose.
## OBJECTIVE
To trace the given track within 30 seconds.
<br> 
![image](https://user-images.githubusercontent.com/47277960/184232380-c6d9c2e7-9a7e-4a6b-b5f9-810a4a2bd073.png)

## EQUIPMENTS USED
### a) SPARK V BOT
![image](https://user-images.githubusercontent.com/47277960/184232833-8205e1f0-79cd-4418-bbc0-44b797302322.png)



### b) A-B cable
![image](https://user-images.githubusercontent.com/47277960/184232979-87cd6115-5444-4f9b-8a7e-32ca1c1ff012.png)


### c) USB ASP AVR Programmer
![image](https://user-images.githubusercontent.com/47277960/184233165-e0631752-caf3-40b6-90e8-248d05ce99e3.png)


### d) Charger
![image](https://user-images.githubusercontent.com/47277960/184233298-2bd5239a-fb8b-4bdd-b25a-de5da1b516b1.png)

## 4 PROCEDURE
• The bot has to be charged (at least 2 hours) completely before
commencing the experiment.
<br>
• The code for IR LED calibration is burned into the bot and it is
placed in the track and a screwdriver is used to rotate the 3
potentiometers on the bot and calibrate the 3 respective IR LEDs.
<br>
• The calibration is done in such a way that the IR LED values
displayed in the LCD panel of the bot show equal values for all 3
LEDs (in both white and black region).
<br>
• Once the calibration is finished, the PID logic is implemented in
the bot and the PID gain values are tuned till the curves are
smoothly traced.
<br>
• The PID output is used to control the velocity of each wheel of the
bot using the Velocity function, where PWM signals which have
duty cycle proportional to the PID output are generated. These
PWM signals are given to the control pins of themotor driver IC.
<br>
• The gain values have to be adjusted to boost the speed of the
bot, in-order to traverse the track within 30 seconds.
<br>
• Thus the track can be traced within the specified time.
