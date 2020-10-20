#include <chrono>	// std::chrono::seconds
#include <cstring> // Needed for strerror
#include <errno.h> // Error number definitions
#include <fcntl.h> // File control definitions
#include <fstream>
#include <iostream> // Standard input/output definitions
#include <string.h> // String function definitions
#include <sys/ioctl.h>
#include <termios.h> // POSIX terminal control definitions
#include <thread>	// std::this_thread::sleep_for
#include <unistd.h>  // UNIX standard function definitions
//#include <asm-generic/termbits.h>

using namespace std;
using namespace std::literals::chrono_literals;

const int uartNum = 1;
const speed_t brate = B1000000;

int system_log(const char *text)
{
	int ret = system(text);
	cout << text << endl;
	return ret;
}

int main(int argc, char *argv[])
{
	/// Using UART 1
	// Shell script found in ...Backseat_Utilities/setComms.sh
	if (int ret = system_log("config-pin p9.24 uart"))
		return ret;
	if (int ret = system_log("config-pin p9.26 uart"))
		return ret;
	if (int ret = system_log("stty -F /dev/ttyO1 sane"))
		return ret;

	cout << "Opening ttyO1" << endl;
	int tty = open("/dev/ttyO1", O_RDWR);
	if (tty == -1)
	{
		auto errMsg = strerror(errno);
		return -1;
	}

	struct termios uartTerm;

	if (tcgetattr(tty, &uartTerm) < 0)
	{
		auto errMsg = strerror(errno);
		return -1;
	}

	cfsetspeed(&uartTerm, B1000000);

	// 8N1
	uartTerm.c_cflag &= ~PARENB;
	uartTerm.c_cflag &= ~CSTOPB;
	uartTerm.c_cflag &= ~CSIZE;
	uartTerm.c_cflag |= CS8;
	uartTerm.c_cflag &= ~CRTSCTS; // no flow control
	//uartTerm.c_cflag &= ~HUPCL; // disable hang-up-on-close to avoid reset
	uartTerm.c_cflag |= CREAD | CLOCAL;					 // turn on READ & ignore ctrl lines
	uartTerm.c_iflag &= ~(IXON | IXOFF | IXANY);		 // turn off s/w flow ctrl
	uartTerm.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
	uartTerm.c_oflag &= ~OPOST;							 // make raw

	// see: http://unixwiz.net/techtips/termios-vmin-vtime.html
	uartTerm.c_cc[VMIN] = 0;
	uartTerm.c_cc[VTIME] = 0;
	//toptions.c_cc[VTIME] = 20;

	tcsetattr(tty, TCSANOW, &uartTerm);
	if (tcsetattr(tty, TCSAFLUSH, &uartTerm) < 0)
	{
		auto errMsg = strerror(errno);
		return -1;
	}

	while (true)
	{
		write(tty, "ping", 5);

		char pong[5] = {0};

		read(tty, pong, 4);

		cout << pong << endl;

		this_thread::sleep_for(1000ms);
	}
}