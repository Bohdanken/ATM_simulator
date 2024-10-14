#include "AccountTest.h"
#include "CardTest.h"
#include "TransferTest.h"

int main()
{
	runAccountTestSuite();
	runCardTestSuite();
	runTransferTestSuite();

	return 0;
}
