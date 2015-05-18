#include "../configurations.h"

typedef vector<int> VI;
typedef vector<VI> VII;


VII build_inverted_index(sp_mat &dat) {
    int n_words = dat.n_rows;

    uvec id = find(dat);

    // Initialize inverted index size
    VI freq(n_words, 0);
    for (int i = 0; i < id.n_elem; i++)
        freq[id[i][0] % n_words]++;

    // Build inverted index
    VII ivt(n_words);
    for (int i = 0; i < n_words; i++)
        ivt[i].reserve(freq[i]);

    for (int i = 0; i < id.n_elem; i++)
        ivt[id[i][0] % n_words].push_back(id[i][0] / n_words);

    return ivt;
}
