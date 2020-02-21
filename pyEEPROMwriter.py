import serial
from serial.tools import list_ports
import time

class pyEEPROM():
	# Simple Python serial interface for the EEPROM programmer
	# Call write(address, data) to write data to an address
	# Call writeArray(address[n], data[n]) to write an array of address an an array of data
	# Call read(address) to read data from an address

	def __init__(self, port = None, baud = None):
		self.__port = port
		self.__baud = baud
		if self.__port != None and self.__baud != None:
			self.start(self.__port, self.__baud)

	def getPorts(self):
		print("Avaliable COM ports are:")
		list_ports.main()
		print("")

	def start(self, port, baud):
		if self.__port != None and self.__baud != None:
			self.__com = serial.Serial(port, baud, timeout = 1)
			time.sleep(0.5)
			self.__com.readline()
			print("COM port set to {} with baud {}. COM port is open: {}".format(port, baud, self.__com.is_open))
			print(" ")

	def close(self):
		print("Closing serial port...", end = '')
		self.__com.close()
		if not self.__com.is_open:
			print(" Success.")
		else:
			print(" Failed")
		print(" ")

	def read(self, address):
		print("Reading entry...")
		data = self.__readAddress(self.__com, address)
		print("Address {] has data {]".format(address, data))
		return data

	def write(self, address, data):
		writeFail = False
		print("Writing entry...")
		check = self.__writeAddress(self.__com, address, data)
		if check != None:
			writeFail = True
		valiFail = False
		print("Validating entry...")
		check = self.__validateAddress(self.__com, address, data)
		if check != None:
			valiFail = True
		return writeFail, valiFail

	def writeArray(self, address, data):
		failedVerification = []
		failedWrite = []
		for i in range(0, len(address)):
			print("Processing entry {} of {} entries...".format(i + 1, len(address)))
			writeFail, valiFail = self.write(address[i], data[i])
			if writeFail:
				failedWrite.append(address[i])
			if valiFail:
				failedVerification.append(address[i])
		return failedWrite, failedVerification

	def __writeAddress(self, com, address, data):
		print("Writing to address {} with data {}... ".format(address, data), end = '')
		output = str(address) + "," + str(data)
		com.write(output.encode())
		time.sleep(0.01)
		reply = com.readline()
		if reply.decode() == "OK\r\n":
			print("OK")
			return None
		print("No reply from programmer... Failed")
		return address

	def __readAddress(sefl, com, address):
		com.flush()
		output = str(address)
		com.write(output.encode())
		time.sleep(0.1)
		reply = com.readline()
		reply = reply.decode()[:-2]
		point = reply.find(",")
		try:
			addr = int(reply[0:point])
			data = int(reply[point + 1::])
		except:
			print("Exception on __readAddress(): Unexpected return.")
			return None
		return data
		
	def __validateAddress(self, com, address, expdata):
		print("Validating address {} with expected data {}... ".format(address, expdata), end = '')
		data = self.__readAddress(self.__com, address)
		print("Found data {}... ".format(data), end = '')
		if data == expdata:
			print("OK")
			return None
		print("Failed")
		return address