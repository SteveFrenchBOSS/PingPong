// set this to the hardware serial port you wish to use
#define PUTTY Serial
#define BBB Serial1

void setup() {
	PUTTY.begin(9600);
	BBB.begin(1000000);
}

void loop() {
  
	if (BBB.available() > 0)
	{
		
		String s = BBB.readString();
		PUTTY.write(s.c_str(), s.length() + 1);
		PUTTY.println();
		//		int incomingByte = BBB.read();
		//		
		//		PUTTY.print("BBB received: ");
		//		PUTTY.println(incomingByte, DEC);
		
		BBB.print("pong");
	}
	
	if (PUTTY.available())
	{
		int incomingByte = PUTTY.read();
		
		PUTTY.print("PUTTY received: ");
		PUTTY.println(incomingByte, DEC);
		
		BBB.print("ping");
		
	}
}