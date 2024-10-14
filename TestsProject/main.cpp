#include "AccountTest.h"
#include "CardTest.h"
#include "mocks/TransferMocks.h"

int main()
{
	runAccountTestSuite();
	runCardTestSuite();

	return 0;
}
