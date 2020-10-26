#define PUTTY Serial
#define BBB Serial1

void setup() {
	PUTTY.begin(9600);
	BBB.begin(1000000);
}

void loop() {
  
	if (BBB.available() > 0)
	{
		
		String ping = BBB.readString();
		PUTTY.write(ping.c_str(), ping.length() + 1);
		PUTTY.println();
		//		int incomingByte = BBB.read();
		//		
		//		PUTTY.print("BBB received: ");
		//		PUTTY.println(incomingByte, DEC);
		
		PUTTY.println("pong");
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