#include "common.h"


int main(int argc, char **argv) {
    /* ./xv_fuzz program -i ./in -o ./out */
    Args args;
    args.parse_args(argc, argv)
        .process();
}