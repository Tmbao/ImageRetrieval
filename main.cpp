#include "configurations.h"
#include "initialize.h"
#include "runserver.h"

#include "app/appdata.h"
#include "extract/extract.h"
#include "quantize/quantize.h"
#include "query/query.h"
#include "utils/score.h"
#include "utils/utils.h"


int main() {

    // Initialize
    extractAll();
    quantizeAllData();

    // Unit test
    // runTest();

    // Run server
    runServer();

    return 0;
}
