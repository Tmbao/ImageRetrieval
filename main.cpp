#include "configurations.h"
#include "initialize.h"
#include "runserver.h"
#include "test.h"

#include "app/appdata.h"
#include "extract/extract.h"
#include "quantize/quantize.h"
#include "query/query.h"
#include "utils/score.h"
#include "utils/utils.h"


int main(int argc, char **argv) {

    // Initialize
    extractAll();
    quantizeAllData();

    if (argc > 1) {
        if (strcmp(argv[1], "test") == 0) {
            runTest();
        } else if (strcmp(argv[1], "runserver")) {
            runServer();
        }
    }

    return 0;
}
