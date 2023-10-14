#include <iostream>
#include "GameController.h"

int main()
{
	system("chcp 1251");
    gameController controller(9, 9);

	controller.PlayerMove();
	/*while (true)
	{
		controller.PlayerMove();
	}*/
}
