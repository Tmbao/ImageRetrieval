#ifndef GEOMETRIC_VERIFICATION_H_INCLUDED
#define GEOMETRIC_VERIFICATION_H_INCLUDED

#include "../configurations.h"

mat map_from_s(vec S) {
    double temp = sqrt(S.at(2)) + eps;

    mat A(2, 2);
    A.at(0, 0) = sqrt(S.at(0) * S.at(2) - sqr(S.at(1))) / temp;
    A.at(1, 0) = 0;
    A.at(0, 1) = S.at(1) / temp;
    A.at(1, 1) = temp;
    return A;
}

mat to_affinity(vec f) {
    if (f.n_rows == 3) {
        // Not implemented yet
    } else if (f.n_rows == 4) {
        // Not implemented yet
    } else if (f.n_rows == 5) {
        A = map_from_s(f.subvec(2, 4));
        A.insert_cols(A.n_cols, f.subvec(0, 1));
        A.insert_rows(A.n_rows, rowvec({0, 0, 1}));
        return A;
    } else if (f.n_rows == 6) {
        // Not implemented yet
    } else {
        stderr << "to_affinity: FAILED" << endl;
    }
}

uvec inliers(mat f1, mat f2, umat matches) {
    int n_matches = matches.n_cols;

    mat x1 = f1.submat(uvec({0, 1}), matches.rows(0).t());
    mat x2 = f2.submat(uvec({0, 1}), matches.rows(1).t());

    mat x1hom = x1;
    x1hom.insert_rows(2, ones<rowvec>(x1.n_cols));
    mat x2hom = x2;
    x2hom.insert_rows(2, ones<rowvec>(x2.n_cols));


    field<uvec> inlrs(n_matches);

    for (int m = 0; m < n_matches; m++) {
        for (int iter = 0; iter < N_ITERATIONS; iter++) {
            double tol;
            mat x1p;
            if (iter == 0) {
                mat a1 = to_affinity(f1.cols(matches.at(0, m)));
                mat a2 = to_affinity(f2.cols(matches.at(1, m)));
                mat a21 = a2 * inv(a1);
                x1p = a21.rows(0, 1) * x1;
                tol = TOLERANCE_1 * sqrt(det(a21.submat(0, 0, 1, 1)));
            } else if (iter <= 4) {
                mat a21 = x2.cols(ret.at(0, m)) / x1hom.cols(ret.at(0, m));
                x1p = a21.rows(0, 1) * x1hom;
                tol = TOLERANCE_2 * sqrt(det(a21.submat(0, 0, 1, 1)));
            } else {
                // Not implemented yet
            }

            mat dist2 = sum((x2 - x1p) % (x2 - x1p), 0);
            inlrs.at(m) = find(dist2 < sqr(tol));

            if (inlrs.at(m).n_elem < MIN_INLIERS)
                break;
            if (inlrs.at(m).n_elem > 0.7 * matches.n_cols)
                break;
        }
    }

    int best = 0;
    for (int i = 1; i < n_matches; i++)
        if (inlrs.at(i).n_elem > inlrs.at(best).n_elem)
            best = i;

    return inlrs.at(best);
}

#endif
