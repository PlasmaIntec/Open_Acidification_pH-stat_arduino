void printFileInDirectory(String dirName, EthernetClient client) {	
	File myDirectory = SD.open(dirName);
	File myFile = myDirectory.openNextFile();
	if (myFile) {
		client.println(myFile.name());

		// read from the file until there's nothing else in it:
		while (myFile.available()) {
			client.write(myFile.read());
		}
		// close the file:
		myFile.close();
	} else {
		// if the file didn't open, print an error:
		client.print("error opening file at directory");
		client.println(dirName);
	}
}