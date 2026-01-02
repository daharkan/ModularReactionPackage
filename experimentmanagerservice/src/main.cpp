#include <cpp_redis/core/client.hpp>
#include <iostream>
#include "ServiceRunner.h"
#include "src/common/Cell.h"
#include "src/common/CellTarget.h"
#include "src/common/Experiment.h"
#include <chrono>
#include <thread>
#include <QCoreApplication>
#include <fstream>

using namespace std;


void writeJSONToFile(const std::string& jsonString, const std::string& filename) {
    std::ofstream outputFile(filename);
    if (outputFile.is_open()) {
        outputFile << jsonString;
        outputFile.close();
        std::cout << "JSON data has been written to " << filename << std::endl;
    } else {
        std::cerr << "Unable to open file " << filename << " for writing." << std::endl;
    }
}

int main(int argc, char *argv[])
{

    cpp_redis::client client;
    client.connect("127.0.0.1", 6379);

/*

    for(int i = 0; i < 9; i++){
        CellTarget celltarget;
        celltarget.setCellID("s25_" + QString::number((i+1)*111).toStdString());
        Document document;
        document.SetObject();

        // Pass the document's allocator to toJSON method to allocate memory for JSON objects
        Document::AllocatorType& allocator = document.GetAllocator();

        Value celltargetJSON = celltarget.toJSON(allocator);
        document.AddMember(DB_TARGETJSON_KEY, celltargetJSON, allocator);

        // Prepare writer and string buffer to convert JSON to string
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        document.Accept(writer);

        // Get the JSON string
        std::string jsonString = buffer.GetString();
        std::future<cpp_redis::reply> reply = client.hset(DB_TARGETTABLE_KEY, celltarget.cellID(), jsonString);
    }
    CellTarget celltarget;
    celltarget.setCellID("s25_101010");
    Document document;
    document.SetObject();

    // Pass the document's allocator to toJSON method to allocate memory for JSON objects
    Document::AllocatorType& allocator = document.GetAllocator();

    Value celltargetJSON = celltarget.toJSON(allocator);
    document.AddMember(DB_TARGETJSON_KEY, celltargetJSON, allocator);

    // Prepare writer and string buffer to convert JSON to string
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);

    // Get the JSON string
    std::string jsonString = buffer.GetString();
    std::future<cpp_redis::reply> reply = client.hset(DB_TARGETTABLE_KEY, celltarget.cellID(), jsonString);
    client.sync_commit();
    return 0;
*/
/*
    for(int i = 0; i < 9; i++){
        Cell cell;
        cell.setCellID("s25_" + QString::number((i+1)*111).toStdString());
        cell.setPositionIdx(i+1);
        Document document;
        document.SetObject();

        // Pass the document's allocator to toJSON method to allocate memory for JSON objects
        Document::AllocatorType& allocator = document.GetAllocator();

        Value cellJSON = cell.toJSON(allocator);
        document.AddMember(DB_CELLJSON_KEY, cellJSON, allocator);

        // Prepare writer and string buffer to convert JSON to string
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        document.Accept(writer);

        // Get the JSON string
        std::string jsonString = buffer.GetString();
        std::future<cpp_redis::reply> reply = client.hset(DB_CELLTABLE_KEY, cell.cellID(), jsonString);
    }
    Cell cell;
    cell.setCellID("s25_101010");
    cell.setPositionIdx(10);
    Document document;
    document.SetObject();

    // Pass the document's allocator to toJSON method to allocate memory for JSON objects
    Document::AllocatorType& allocator = document.GetAllocator();

    Value cellJSON = cell.toJSON(allocator);
    document.AddMember(DB_CELLJSON_KEY, cellJSON, allocator);

    // Prepare writer and string buffer to convert JSON to string
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);

    // Get the JSON string
    std::string jsonString = buffer.GetString();
    std::future<cpp_redis::reply> reply = client.hset(DB_CELLTABLE_KEY, cell.cellID(), jsonString);
    client.sync_commit();


    return 0;
*/

    QCoreApplication app(argc, argv); // Create a QCoreApplication instance to run the event loop

    ServiceRunner serviceRunner;
    while (true) {
        if (serviceRunner.initService()) {
            try {
                serviceRunner.startServiceLoop();
            }
            catch (...) {
                continue;
            }
        }
        std::cout << "waiting for busboard..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        // Process events in the Qt event loop
        app.processEvents();
    }

    return app.exec(); // This will never be reached in a non-GUI application
}
