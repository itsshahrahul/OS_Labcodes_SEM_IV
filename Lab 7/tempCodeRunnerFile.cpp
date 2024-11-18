#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
using namespace std;
int main() {
    int i, n, m, sum = 0, h;
    cout << "Enter the size of disk: ";
    cin >> m;
    cout << "Enter the number of requests: ";
    cin >> n;
    vector<int> requests(n), seek_sequence;
    cout << "Enter the requests:\n";
    for (i = 0; i < n; i++) {
        cin >> requests[i];
    }
    for (i = 0; i < n; i++) {
        if (requests[i] > m) {
            cout << "Error: Unknown position " << requests[i] << "\n";
            return 0;
        }
    }
    cout << "Enter the head position: ";
    cin >> h;
    requests.push_back(h);
    requests.push_back(m);
    requests.push_back(0);
    sort(requests.begin(), requests.end());
    auto it = find(requests.begin(), requests.end(), h);
    int index = distance(requests.begin(), it);
    if (index < n / 2) {
        for (i = index; i < requests.size(); i++) {
            seek_sequence.push_back(requests[i]);
        }
        seek_sequence.push_back(m);
        for (i = index - 1; i >= 0; i--) {
            seek_sequence.push_back(requests[i]);
        }
    } else {
        for (i = index; i >= 0; i--) {
            seek_sequence.push_back(requests[i]);
        }
        seek_sequence.push_back(0); 
        for (i = index + 1; i < requests.size(); i++) {
            seek_sequence.push_back(requests[i]);
        }
    }
    int temp = seek_sequence[0];
    cout << temp;
    for (i = 1; i < seek_sequence.size(); i++) {
        cout << " -> " << seek_sequence[i];
        sum += abs(seek_sequence[i] - temp);
        temp = seek_sequence[i];
    }
    cout << '\n';
    cout << "Total head movements = " << sum << '\n';
    cout << "Average head movement = " << static_cast<float>(sum) / n << '\n';    
    return 0;
}
