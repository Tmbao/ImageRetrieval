#include "../configurations.h"


class app_data {
public:

    static app_data* get_instance() {
        if (_instance == NULL)
            _instance = new app_data();

        return _instance;
    }

    // TODO: 
    // - Implement methods for loading tf_idf and keypoint from storage to memory
    // - Build inverted index for tf_idf

private:
    // Singleton instance
    static app_data* _instance;

    // Prohibited methods
    app_data() {}
    app_data(app_data const &) {}
    app_data& operator = (app_data const &) {}

public:

    int n_docs;
    sp_mat tf_idf;

    // TODO: add inverted index, keypoint
};
