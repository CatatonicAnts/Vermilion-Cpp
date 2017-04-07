#include "common.hpp"

int main(int argc, char * * argv)
{
    std::cout << "Harro, word!" << std::endl;

    std::ifstream t("tests/code1.ver");
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size);

    std::cout << "Preparing tokenizer." << std::endl;

    auto tkgen = new verm::tokenizer("tests/code1.ver", buffer);
    auto cons = new verm::token_consumer(tkgen);

    std::cout << "Fetching tokens." << std::endl;

    verm::line_of_code * lastLine = nullptr;
    for (verm::token * tk; true; /* nothing */)
    {
        tk = cons->fetch_token();

        if (tk != nullptr)
        {
            if (tk->get_position().line != lastLine)
            {
                lastLine = tk->get_position().line;

                std::cout << "Line " << lastLine->get_number() << std::endl;
            }

            std::cout << *tk << std::endl;

            if (tk->get_type() == verm::tk_end_of_file)
                break;
        }
        else
        {
            std::cout << "FAILED TO PRODUCE TOKEN:" << std::endl;
            std::cout << cons->get_error() << std::endl;

            break;
        }
    }

    std::cout << "Cleaning up." << std::endl;

    delete cons;
    delete tkgen;

    return 0;
}
