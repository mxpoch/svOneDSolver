#include <cvOneDFEAVector.h>

struct cvOneDFEAVectorState {
    double normf;
    double norms;
    std::vector<long> equationNumbers;
    std::vector<double> entries;
};

class cvOneDFEAVectorHistory {
    public:
        cvOneDFEAVectorHistory();
        std::vector<cvOneDFEAVectorState> buffer;
        void RecordStep(const cvOneDFEAVector& vec, double normf,  double norms);
        void WriteToFile(const std::string& filename);
};