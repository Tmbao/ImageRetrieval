#ifndef IMAGE_RETRIEVAL_APP_DATA_H
#define IMAGE_RETRIEVAL_APP_DATA_H


#include "../configurations.h"
#include "../utils/inverted_index.h"


class app_data {
public:

    inverted_index ivt;

private:

    static app_data* _instance;

    app_data(): ivt(number_of_visual_words) {}
    app_data(app_data const &) {}
    app_data& operator = (app_data const &) {}

public:

    static app_data* get_instance() {
        if (_instance == NULL)
            _instance = new app_data();

        return _instance;
    }

};

app_data* app_data::_instance = NULL;


#endif IMAGE_RETRIEVAL_APP_DATA_H