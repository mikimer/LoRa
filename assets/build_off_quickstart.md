# Build off of the LoRa-Arduino quickstart project

Here are tips, tricks, and other resources:

* The Arduino sketch only tries to join the LoRa network in the beginning of the program, not continuously -- it issues the AT+JOIN command in setup(), not in loop().  

* If you change the code, there is a huge difference between the print() and println() commands. The Arduino issues a carriage return (`Enter`) after a println(), but not after a print().   

* [Include a Particle Spark version of the code from Andrew @ Sutro]


