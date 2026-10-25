#include "fasta_utils.h"
#include <fstream>


std::string read_sequences_from_fasta(const std::string &filename,
std::vector<std::string> &names) {
std::ifstream file(filename);
if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::string sequences = "$";
    std::string line, seq_name, sequence;

 auto process_sequence = [&](const std::string &seq,
                                const std::string &name) {
        names.push_back(name);
        sequences += seq + '$';
    };

    while (std::getline(file, line)) {
        if (!line.empty() && line[0] == '>') {
            if (!sequence.empty()) {
                process_sequence(sequence, seq_name);
                sequence.clear();
            }
            seq_name = line.substr(1);
        } else if (!line.empty()) {
            sequence += line;
        }
    }

    if (!sequence.empty()) {
        process_sequence(sequence, seq_name);
    }

    return sequences;
}
