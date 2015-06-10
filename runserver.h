#ifndef RUNSERVER_H_INCLUDED
#define RUNSERVER_H_INCLUDED

#include "configurations.h"
#include "app/appdata.h"
#include "extract/extract.h"
#include "quantize/quantize.h"
#include "query/query.h"
#include "utils/score.h"
#include "utils/utils.h"


// Import mysql 
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace sql;

#include <thread>
#include <unistd.h>


struct Request {
    int id;
    string fileName;
    Request(int _id = 0, string _fileName = ""): id(_id), fileName(_fileName) {}
};


Driver *driver;
Connection *con;


void processQuery(string fileName) {

    Statement *stmt;
    stmt = con->createStatement();

    AppData *app = AppData::getInstance();
    int nDocs = app->path.size();

    string kpPath = fileName + ".kp.mat";
    string siftPath = fileName + ".sift.mat";
    string tempPath = fileName + ".temp.mat";
    string weightPath = fileName + ".weight.mat";
    string termIDPath = fileName + ".term.mat";
    string imgPath = fileName;

    mat _kp, _sift;
    extractFeatures(imgPath, _kp, _sift, kpPath, siftPath, tempPath);

    vec _weights;
    uvec _termID;
    buildBoW(_sift, _weights, _termID, weightPath, termIDPath, true);

    vector<double> qTfidf = app->ivt.makeQueryTfidf(_weights, _termID);

    // Compute ranked list
    Score score(computeAllScores(qTfidf));
    vector<int> rankedList(nDocs);
    for (int i = 0; i < nDocs; i++)
        rankedList[i] = i;
    sort(rankedList.begin(), rankedList.end(), score);

    vector<string> rankedListStr(nDocs);
    for (int i = 0; i < nDocs; i++)
        rankedListStr[i] = getFileBaseName(app->path[rankedList[i]]);

    // Save ranked list into db
    string jsonRankedList = vectorToJson(rankedListStr, "ranked_list");
    string command = "insert into ImageRetrieval_responserankedlist(data) values('" + jsonRankedList + "')";
    stmt->executeQuery(command.c_str());
}

void runServer() {
    Statement *stmt;
    ResultSet *res;

    try {

        // Create connection
        driver = get_driver_instance();
        con = driver->connect(sqlUrl, sqlUser, sqlPassword);
        // Connect to database
        con->setSchema("ImageRetrievalServer");
        stmt = con->createStatement();

        while (true) {
            vector<Request> requests;

            // Fetching all current requests
            res = stmt->executeQuery("select * from ImageRetrieval_imagedocument");
            while (res->next()) 
                requests.push_back(Request(parseInt(res->getString(1)), res->getString(2)));
            delete res;

            if (!requests.empty()) {
                // Delete fetched requests
                for (Request request : requests) {
                    string command  = "delete from ImageRetrieval_imagedocument where id = " + to_string(request.id);
                    stmt->executeQuery(command.c_str());
                }

                // Process requests
                for (Request request : requests) {
                    // Create thread
                    thread tQuery(processQuery, mediaRoot + request.fileName);
                    tQuery.join();
                }
            }

            usleep(loopSleepTime);
        }

    } catch (sql::SQLException &e) {
        cerr << "# ERR: SQLException in " << __FILE__;
        cerr << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cerr << "# ERR: " << e.what();
        cerr << " (MySQL error code: " << e.getErrorCode();
        cerr << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
}

#endif
