#include <bits/stdc++.h>

using namespace std;
const int MEMORY = 90 * 1000 * 1000;
const int OPENED_SIM = 25;

void flush(vector<string>& vs, int& file_num) {
    ofstream fout(to_string(file_num));
    ++file_num;
    sort(vs.begin(), vs.end());
    for (auto& line : vs) {
        if (line != "") {
            fout << line << "\n";
        }
    }
    vs.clear();
}

// split 'input.txt' to ['0'; split())
int split() {
    // Timer T("split");
    ifstream fin("input.txt");
    int file_num = 0;
    int total_len = 0;
    vector<string> vs;
    string tmp;
    ////
    while (getline(fin, tmp)) {
        if (tmp == "") {
            continue;
        }
        total_len += tmp.size();
        vs.push_back(tmp);
        if (total_len > MEMORY) {
            flush(vs, file_num);
            total_len = 0;
        }
    }
    if (vs.size() != 0) {
        flush(vs, file_num);
    }
    return file_num;
}

// files from ['first'; 'last') merge to 'answername'
void merge(int first, int last, int answername) {
    // Timer T("merge");
    multiset<pair<string, int> > merge_queue;
    vector<ifstream> files(static_cast<size_t>(last - first));

    for (int i = first; i < last; ++i) {
        files[i - first].open(to_string(i));

        string s;
        getline(files[i - first], s);
        merge_queue.insert({s, i - first});
    }
    ofstream answer(to_string(answername));

    while (!merge_queue.empty()) {
        auto pairr = *merge_queue.begin();
        merge_queue.erase(merge_queue.begin());

        answer << pairr.first << "\n";

        if (getline(files[pairr.second], pairr.first)) {
            if (pairr.first != "") {
                merge_queue.insert(pairr);
            }
        }
    }

    // cleanup
    for (int i = first; i < last; ++i) {
        files[i - first].close();
        remove(to_string(i).c_str());
    }
}

int main() {
    int first = 0;
    int last = split();
    while (last - first > 1) {
        int old_first = first;
        int old_last = last;
        for (int i = old_first; i < old_last; i += OPENED_SIM) {
            merge(i, min(i + OPENED_SIM, old_last), last);
            ++last;
        }
        first = old_last;
    }
    rename(to_string(first).c_str(), "output.txt");
}