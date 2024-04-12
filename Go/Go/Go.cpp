#include <iostream>
#include "GameController.h"


int main()
{
	system("chcp 1251");
    gameController controller(10, 10);

	controller.PlayerMove();
	controller.GetResults();

	return 0;
}
