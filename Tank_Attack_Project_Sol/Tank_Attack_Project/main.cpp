#include <cstdlib>
#include <ctime>
#include "Presentador.h"

int main() {
    srand(time(NULL));

    Presentador presentador;
    if (!presentador.init()) return -1;
    presentador.run();

    return 0;
}