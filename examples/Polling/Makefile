SKETCH = Polling

PORT = /dev/ttyACM0

FQBN = teensy:avr:teensy40
# FQBN = esp32:esp32:um_tinypico

LIBS = ../..

build: $(SKETCH).ino 
	arduino-cli compile  --libraries $(LIBS) --fqbn $(FQBN) $(SKETCH).ino

flash:
	arduino-cli upload -p $(PORT) --fqbn $(FQBN) .

upload:
	arduino-cli upload -p $(PORT) --fqbn $(FQBN) .

edit:
	vim $(SKETCH).ino

listen:
	miniterm.py $(PORT) 115200 --exit-char 3 # exit on CTRL-C
