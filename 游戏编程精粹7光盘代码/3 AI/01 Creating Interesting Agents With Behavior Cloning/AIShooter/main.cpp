#include <iostream>
#include <exception>
#include <stdexcept>

#include "AIShooter.h"

int main (int argc, char **argv)
{
    try {
        AIShooter *game = AIShooter::getInstance ();

        game->init (argc, argv);
        game->run ();
    } catch (std::exception &e) {
        std::cerr << "Exception caught: " << e.what () << std::endl;
        return 1;
    }

    return 0;
}
