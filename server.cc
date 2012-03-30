#include <iostream>
#include <sstream>
#include <sstream>

using namespace std;

int
main(int argc, const char *argv[])
{
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " PORT" << endl;
        return 1;
    }

    int port;
    istringstream(argv[1]) >> port;

    cout << port << endl;

    return 0;
}
