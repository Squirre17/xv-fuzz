#include "common.h"



int main(int argc, char *argv[]) {
    /* ./xv_fuzz program -i ./in -o ./out */
    Args args;
    args.parse_clang_args(argc, argv)
        .process()
        .show_params()
        .exec();
}