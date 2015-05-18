#include "../configurations.h"
#include "../data/app_data.h"


vector<double> compute_all_distances(sp_mat query) {
    app_data* app = app_data::get_instance();

    sp_mat dist = query.t() * app->tf_idf;

    vector<double> ret(app->n_docs);
    for (int i = 0; i < app->n_docs; i++)
        ret[i] = dist.at(0, i);
    return ret;
}
