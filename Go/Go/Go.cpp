#include <iostream>
#include "GameController.h"

int main()
{
	system("chcp 1251");
    gameController controller(5, 5);

	while (true)
	{
		controller.PlayerMove();
	}
}
