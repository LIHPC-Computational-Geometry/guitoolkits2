#include <iostream>
#include <string>
#include <QtUtil/QtUtilInfos.h>

int main() {
    std::cout << "Version " << QtUtilInfos::getVersion().getMajor() << std::endl;
    return 0;
}
