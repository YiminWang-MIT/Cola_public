//
// Created 1995 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/FourVector/testMultiple.cc,v 3.3 2002-11-26 18:11:50 distler Exp $
//
// Test Programm fuer den multiple scattering Generator
//

#include "Multiple.h"
#include <stdio.h>
#include <time.h>
#include <iostream>

int
main(int argc, char *argv[])
{
    ACG gen(time(0),20);
    int times = 10;

    if (argc == 2) sscanf(argv[1],"%d",&times);

    Multiple mscatter(0.01, &gen);

    while (times--)
	std::cout << mscatter() << std::endl;

    return 0;
}
