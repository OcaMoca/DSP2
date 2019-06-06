
#ifndef _K_MEANS_
#define _K_MEANS_

#include <QtGlobal>
#include <vector>


// Function for kMeans centorid calculation
std::vector<std::vector<double>> kMeansCentroids(std::vector<std::vector<double>> points, int nFeatures, int K);

// Find nearest centorid (helper function)
int whichIsNearest(const std::vector<std::vector<double>>& centroids, const std::vector<double>& pt);


#endif //  _K_MEANS_
