#!/bin/sh

# -std=c++11 -O3 -Wall -Wextra -Werror

#g++ -o dnj_femas.out dnj_femas.cpp dnj_femas_common.cpp dnj_femas_api_input.cpp dnj_femas_spi_output.cpp dnj_femas_console.cpp -I./ -I/home/jie/cffex/api_af/lnx64/ -I/usr/include/ -L/usr/lib/ -L/home/jie/cffex/api_af/lnx64 -lUSTPmduserapiAF -lUSTPtraderapiAF -pthread

g++ -o dnj_femas.out dnj_femas.cpp dnj_femas_common.cpp dnj_femas_api_input.cpp dnj_femas_spi_output.cpp dnj_femas_console.cpp -I./ -I/home/jie/cffex/api_af/lnx64/ -I/usr/include/ -L/usr/lib/ -L/home/jie/cffex/api_af/lnx64 -lUSTPmduserapiAF -lUSTPtraderapiAF -pthread
