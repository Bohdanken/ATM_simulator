#include "AccountTest.h"
#include "CardTest.h"
#include "TransferTest.h"
#include "TransferDaemonTest.h"

int main()
{
	runAccountTestSuite();
	runCardTestSuite();
	runTransferTestSuite();
	testTransferDaemon();

	runAccountTestSuite();
	return 0;

}
