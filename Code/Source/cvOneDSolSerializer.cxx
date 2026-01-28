#include <iostream>
#include <fstream>
#include <cvOneDFEAVector.h>
#include <cvOneDSolSerializer.h>

// constructor / destructor
cvOneDFEAVectorHistory::cvOneDFEAVectorHistory(){
    // reserving memory for the first few iterations
    buffer.reserve(100);
}

void cvOneDFEAVectorHistory::RecordStep(const cvOneDFEAVector& vec, double normf, double norms) {
    // capturing the data
    cvOneDFEAVectorState state;
    long dimension = vec.GetDimension();

    // saving the residuals.
    state.normf = normf;
    state.norms = norms;

    // deep copy the pointer data into vectors
    state.equationNumbers.assign(vec.GetEquationNumbers(), vec.GetEquationNumbers() + dimension);
    state.entries.assign(vec.GetEntries(), vec.GetEntries() + dimension);

    buffer.push_back(state);
}

void cvOneDFEAVectorHistory::WriteToFile(const std::string& filename) {
    // Open in binary mode. ios::out is implied for ofstream.
    std::ofstream outFile(filename, std::ios::binary);

    if (!outFile) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }

    for (const auto& state : buffer) {
        // 1. Calculate the dimension (N)
        long dim = static_cast<long>(state.entries.size());

        // 2. Write the scalars (8 bytes each)
        outFile.write(reinterpret_cast<const char*>(&state.normf), sizeof(double));
        outFile.write(reinterpret_cast<const char*>(&state.norms), sizeof(double));

        // 3. Write the dimension (8 bytes) - The "Key" for the vectors below
        outFile.write(reinterpret_cast<const char*>(&dim), sizeof(long));

        // 4. Write Equation Numbers (N * 8 bytes)
        outFile.write(reinterpret_cast<const char*>(state.equationNumbers.data()), 
                      dim * sizeof(long));

        // 5. Write Entries (N * 8 bytes)
        outFile.write(reinterpret_cast<const char*>(state.entries.data()), 
                      dim * sizeof(double));
    }

    outFile.close();
}





