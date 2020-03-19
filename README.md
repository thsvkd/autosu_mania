# autosu_maina

### I made osu! mania auto macro which can run at arduino micro board. This project consists of two main components.

    1. Aduino code generator
    2. base Aduino code 

### ***The code generator*** running at your PC, and uses the ***base aduino code*** to generate a compilationable aduino code and copy it to the clipboard. And then paste the code into the aduino IDE and upload it to the aduino micro. ***(use ctrl + A, ctrl + V)***

### Once the upload is complete, press the button corresponding to ***pin 5*** to start the auto play. If you press the button corresponding to ***pin 4***, automatic play will stop. 

### ***Pin 6*** has been assigned as an LED pin, and if the LED lights up, it's in auto play mode and if it's off, it's in controller mode (to be updated in the future).

### ***Pin 3*** and ***pin 2*** respectively slow down or quicken the sync of the automatic play. In the osu! setting menu, if the score meter of the game play section is set to ***"judgment error"***, ***pin 3*** moves the white bar to the left and ***pin 2*** moves to the right.
