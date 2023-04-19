double gaussV1(double A[][SIZE], const double b[], double x[], size_t n, double eps) {
    int perm[n];
    int curr;
    int j;
    print_mat(A, n, n);
    for (int i = 0; i < n; i++) {
        perm[i] = i;
    }
    double multip, det = 1.0;
    for (int i = 0; i < n; i++) {
        {
            curr = change_with_highest(i, i, A, perm, n);
            if (A[curr][i] < eps)
                return 0;
            det = det * A[curr][i];
            j = i;
            while (j < n) {
                if (perm[j] != curr) {
                    multip = A[perm[j]][i] / A[curr][i];
                    for (int k = 0; k < n; k++) {
                        A[perm[j]][k] = A[perm[j]][k] - A[curr][k] * multip;
                    }
//                    print_mat(A, n, n);
                }
                j++;
            }
        }
//        print_mat(A, n, n);
    }
    return det;
}