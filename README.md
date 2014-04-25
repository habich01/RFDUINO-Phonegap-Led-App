RFDUINO-Phonegap-Led-App
========================

The sketch combines LED example with Connection example.

1) it uses both Button A and Button B
-Button B turns on/off BLE
-Button A sends a signal to the App

2) LED colors
- LED is white when BLE is off
- Red when BLE is advertising
- Green when connected
- Blue when App turns on the LED

NOTES on sketch

- RFduino.send is now RFduino.sendFloat
- pinWake Low doesn't seem to allow more than 1 button therefore a button tracker is used
- using a comination of strings to int etc ... allows flexibility when re-purposing the code

***********************************************************************************************************************
The app is a phonegap app 

NOTES on app

I forze the orientation of the app on android to portrait because of the div box
