#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char *argv[])
{
    const std::string hello = "Hello";
    const std::string message1 = hello + ", world" + "!";

    std::cout << message1 << std::endl;

    const std::string exclam = "!";
    const std::string message2 = "Hello" + ", world" + exclam;

    std::cout << message2 << std::endl;
}
